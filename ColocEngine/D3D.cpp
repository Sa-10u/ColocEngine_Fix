#include "D3D.h"
#include<cassert>
#include"ResourceUploadBatch.h"
#include "DDSTextureLoader.h"
#include "VertexTypes.h"
#include <string>
#include"ResourceManager.h"
#include"FileLoader.h"
#include"CAM.h"
#include"S_Draw.h"

constexpr UINT CBCOUNT = 256;

bool D3d::Initialize(HWND hwnd, uint32_t h, uint32_t w)
{
    PTR_D3D::ptr = this;
    PTR_WND::ptr = &hwnd;

    backcolor_[0] = .0f;
    backcolor_[1] = .0f;
    backcolor_[2] = .5f;
    backcolor_[3] = 1.0f;

    for (auto i = 0u; i < FrameAmmount; ++i) {

        colbuf_[i] = nullptr;
        cmdalloc_[i] = nullptr;
        fencecnt_[i] = NULL;
    }

    this->Height = h;
    this->Width = w;

    bool FAIL = 0;

    auto res = CoInitializeEx(NULL, COINITBASE_MULTITHREADED);
    if (FAILED(res))               return FAIL;

    res = D3D12CreateDevice
    (
        nullptr,
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS((&device_))
    );
    if (FAILED(res))               return FAIL;


    D3D12_COMMAND_QUEUE_DESC cmddesc = {};
    {
        cmddesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        cmddesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        cmddesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        cmddesc.NodeMask = NULL;
    }
    res = device_->CreateCommandQueue(&cmddesc, IID_PPV_ARGS((&cmdque_)));
    if (FAILED(res))     return FAIL;


    IDXGIFactory4* fact = nullptr;
    res = CreateDXGIFactory1(IID_PPV_ARGS((&fact)));
    if (FAILED(res))     return 0;


    DXGI_SWAP_CHAIN_DESC desc = {};
    {
        desc.BufferDesc.Height = Height;
        desc.BufferDesc.Width = Width;
        desc.BufferDesc.RefreshRate.Numerator = 60;
        desc.BufferDesc.RefreshRate.Denominator = 1;
        desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = FrameAmmount;
        desc.OutputWindow = hwnd;
        desc.Windowed = TRUE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    }
    IDXGISwapChain* p_swch = nullptr;
    res = fact->CreateSwapChain(cmdque_, &desc, &p_swch);
    if (FAILED(res))
    {
        SAFE_RELEASE(fact);
        return FAIL;
    }

    res = p_swch->QueryInterface(IID_PPV_ARGS((&swpchain_)));
    if (FAILED(res))
    {
        return FAIL;
    }

    IND_frame = swpchain_->GetCurrentBackBufferIndex();

    SAFE_RELEASE(fact);
    SAFE_RELEASE(p_swch);


    for (int i = 0u; i < FrameAmmount; ++i) {

        res = device_->CreateCommandAllocator
        (
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS((&cmdalloc_[i]))
        );

        if (FAILED(res)) return FAIL;
    }

    res = device_->CreateCommandList
    (
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        cmdalloc_[IND_frame],
        nullptr,
        IID_PPV_ARGS((&cmdlist_))
    );
    if (FAILED(res)) return FAIL;

    D3D12_DESCRIPTOR_HEAP_DESC hpdesc;
    {
        hpdesc.NumDescriptors = FrameAmmount;
        hpdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        hpdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        hpdesc.NodeMask = 0;
    }
    res = device_->CreateDescriptorHeap(&hpdesc, IID_PPV_ARGS((&heapRTV_)));
    if (FAILED(res))    return FAIL;

    D3D12_CPU_DESCRIPTOR_HANDLE handle = heapRTV_->GetCPUDescriptorHandleForHeapStart();
    UINT incre = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    for (UINT i = 0u; i < FrameAmmount; ++i) {

        res = swpchain_->GetBuffer(i, IID_PPV_ARGS(&colbuf_[i]));
        if (FAILED(res))     return FAIL;

        D3D12_RENDER_TARGET_VIEW_DESC rtvdesc = {};
        {
            rtvdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
            rtvdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            rtvdesc.Texture2D.MipSlice = NULL;
            rtvdesc.Texture2D.PlaneSlice = NULL;
        }

        device_->CreateRenderTargetView(colbuf_[i], &rtvdesc, handle);

        h_RTV[i] = handle;
        handle.ptr += incre;
    }

    {
        D3D12_HEAP_PROPERTIES hp_prop_z = {};
        {
            hp_prop_z.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            hp_prop_z.CreationNodeMask = 1;
            hp_prop_z.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            hp_prop_z.Type = D3D12_HEAP_TYPE_DEFAULT;
            hp_prop_z.VisibleNodeMask = 1;
        }

        D3D12_RESOURCE_DESC rc_desc_z = {};
        {
            rc_desc_z.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            rc_desc_z.Format = DXGI_FORMAT_D32_FLOAT;
            rc_desc_z.MipLevels = 1;
            rc_desc_z.Alignment = 0;
            rc_desc_z.DepthOrArraySize = 1;
            rc_desc_z.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
            rc_desc_z.Height = Height;
            rc_desc_z.Width = Width;
            rc_desc_z.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            rc_desc_z.SampleDesc.Count = 1;
            rc_desc_z.SampleDesc.Quality = 0;
        }

        D3D12_CLEAR_VALUE clearcol = {};
        {
            clearcol.Format = DXGI_FORMAT_D32_FLOAT;
            clearcol.DepthStencil.Depth = 1.0f;
            clearcol.DepthStencil.Stencil = 0;
        }

        device_->CreateCommittedResource
        (
            &hp_prop_z,
            D3D12_HEAP_FLAG_NONE,
            &rc_desc_z,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &clearcol,
            IID_PPV_ARGS(&ZB)
        );
        //-----

        D3D12_DESCRIPTOR_HEAP_DESC hp_desc = {};
        {
            hp_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            hp_desc.NodeMask = 0;
            hp_desc.NumDescriptors = 1;
            hp_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        }

        res = device_->CreateDescriptorHeap
        (
            &hp_desc,
            IID_PPV_ARGS(&hp_ZBV)
        );
        if (FAILED(res)) return 0;

        auto handle = hp_ZBV->GetCPUDescriptorHandleForHeapStart();
        auto incre = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

        D3D12_DEPTH_STENCIL_VIEW_DESC zbv_desc = {};
        {
            zbv_desc.Flags = D3D12_DSV_FLAG_NONE;
            zbv_desc.Format = DXGI_FORMAT_D32_FLOAT;
            zbv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            zbv_desc.Texture2D.MipSlice = 0;
        }

        device_->CreateDepthStencilView
        (
            ZB,
            &zbv_desc,
            handle
        );

        h_ZBV = handle;
    }


    {
        for (auto i = 0u; i < FrameAmmount; ++i) {
            fencecnt_[i] = 0;

            res = device_->CreateFence
            (
                fencecnt_[IND_frame],
                D3D12_FENCE_FLAG_NONE,
                IID_PPV_ARGS(&fence_)
            );

            if (FAILED(res))     return FAIL;

            fencecnt_[IND_frame]++;

            event_fence = CreateEvent(nullptr, false, false, nullptr);
            if (event_fence == nullptr)      return FAIL;

        }
    }

    cmdlist_->Close();
    InitGBO();

    __CREATE("Pipeline State Object : PSO")
    {
        if (!InitPSO())      return 0;
    }

    return true;
}

bool D3d::InitGBO()
{
    HRESULT res = FALSE;

    //----------------------------
    {
        {
            D3D12_DESCRIPTOR_HEAP_DESC hp_desc = {};
            {
                hp_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
                hp_desc.NodeMask = 0;
                hp_desc.NumDescriptors = 3 * FrameAmmount;
                hp_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            }

            res = device_->CreateDescriptorHeap
            (
                &hp_desc,
                IID_PPV_ARGS(&heapCBV_SRV_UAV_)
            );

            if (FAILED(res)) return false;
        }
    }

    //------------------------
    {
        D3D12_HEAP_PROPERTIES hp_proc_c = {};
        {
            hp_proc_c.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            hp_proc_c.Type = D3D12_HEAP_TYPE_UPLOAD;
            hp_proc_c.CreationNodeMask = 1;
            hp_proc_c.VisibleNodeMask = 1;
            hp_proc_c.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        }

        D3D12_RESOURCE_DESC rc_desc_c = {};
        {
            rc_desc_c.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            rc_desc_c.Format = DXGI_FORMAT_UNKNOWN;
            rc_desc_c.MipLevels = 1;
            rc_desc_c.Alignment = 0;
            rc_desc_c.Height = 1;
            rc_desc_c.Width = sizeof(Util);
            rc_desc_c.DepthOrArraySize = 1;
            rc_desc_c.Flags = D3D12_RESOURCE_FLAG_NONE;
            rc_desc_c.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

            rc_desc_c.SampleDesc.Count = 1;
            rc_desc_c.SampleDesc.Quality = 0;
        }

        auto incre = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        for (auto i = 0; i < FrameAmmount; ++i) {

            res = device_->CreateCommittedResource
            (
                &hp_proc_c,
                D3D12_HEAP_FLAG_NONE,
                &rc_desc_c,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                NULL,
                IID_PPV_ARGS(&CB[i])
            );
            if (FAILED(res))     return false;
            //----------------------------------

            auto address = CB[i]->GetGPUVirtualAddress();
            auto _HCPU = heapCBV_SRV_UAV_->GetCPUDescriptorHandleForHeapStart();
            auto _HGPU = heapCBV_SRV_UAV_->GetGPUDescriptorHandleForHeapStart();

            _HCPU.ptr += incre * i;
            _HGPU.ptr += incre * i;

            CBV[i].HCPU = _HCPU;
            CBV[i].HGPU = _HGPU;
            CBV[i].desc.BufferLocation = address;
            CBV[i].desc.SizeInBytes = sizeof(Util);

            device_->CreateConstantBufferView(&CBV[i].desc, _HCPU);

            res = CB[i]->Map(0, NULL, reinterpret_cast<void**>(&CBV[i].ptr));
            if (FAILED(res)) return 0;
        }
    }
    //-----------------------------------------------------------------------------------------*****
    {
        std::wstring tex_Path;
        FileLoad(L"default.dds", &tex_Path);

        TexMetadata data = {};
        ScratchImage image = {};
        const Image* rsc = nullptr;

        res = LoadFromWICFile
        (
            tex_Path.c_str(),
            WIC_FLAGS_NONE,
            &data,
            image
        );
      
        rsc = image.GetImage(0, 0, 0);


        D3D12_RESOURCE_DESC rc_desc_tex = {};
        {
            rc_desc_tex.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            rc_desc_tex.Format = rsc->format;
            rc_desc_tex.MipLevels = 1;
            rc_desc_tex.DepthOrArraySize = 1;
            rc_desc_tex.Flags = D3D12_RESOURCE_FLAG_NONE;
            rc_desc_tex.Height = rsc->height;
            rc_desc_tex.Width = rsc->width;
            rc_desc_tex.SampleDesc.Count = 1;
            rc_desc_tex.SampleDesc.Quality = 1;
            rc_desc_tex.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        }

        D3D12_HEAP_PROPERTIES hp_prop_tex = {};
        {
            hp_prop_tex.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
            hp_prop_tex.CreationNodeMask = 0;
            hp_prop_tex.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
            hp_prop_tex.Type = D3D12_HEAP_TYPE_CUSTOM;
            hp_prop_tex.VisibleNodeMask = 0;
        }

        res = device_->CreateCommittedResource
        (
            &hp_prop_tex,
            D3D12_HEAP_FLAG_NONE,
            &rc_desc_tex,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&tex.rsc_ptr)
        );
        if (FAILED(res)) return 0;

        res = tex.rsc_ptr->WriteToSubresource
        (
            0,
            nullptr,
            rsc->pixels,
            rsc->rowPitch,
            rsc->slicePitch
        );
        if (FAILED(res)) return 0;

        //auto HCPU_ = heapTex_->GetCPUDescriptorHandleForHeapStart();
       // auto HGPU_ = heapTex_->GetGPUDescriptorHandleForHeapStart();
        auto HCPU_ = heapCBV_SRV_UAV_->GetCPUDescriptorHandleForHeapStart();
        auto HGPU_ = heapCBV_SRV_UAV_->GetGPUDescriptorHandleForHeapStart();
        auto incre = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        tex.HCPU.ptr = HCPU_.ptr + incre * 2;
        tex.HGPU.ptr = HGPU_.ptr + incre * 2;


        D3D12_SHADER_RESOURCE_VIEW_DESC rsc_v_desc = {};
        {
            rsc_v_desc.Format = rc_desc_tex.Format;
            rsc_v_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            rsc_v_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            rsc_v_desc.Texture2D.MipLevels = rc_desc_tex.MipLevels;
            rsc_v_desc.Texture2D.MostDetailedMip = 0;
            rsc_v_desc.Texture2D.PlaneSlice = 0;
            rsc_v_desc.Texture2D.ResourceMinLODClamp = 0.0f;
        }

        device_->CreateShaderResourceView
        (
            tex.rsc_ptr,
            &rsc_v_desc,
            tex.HCPU
        );
    }
    //-----------------******

     //StructuredBuffer
    {
        D3D12_HEAP_PROPERTIES hp_prop = {};
        {
            hp_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            hp_prop.CreationNodeMask = 1;
            hp_prop.VisibleNodeMask = 1;
            hp_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            hp_prop.Type = D3D12_HEAP_TYPE_UPLOAD;
        }

        D3D12_RESOURCE_DESC rc_desc = {};
        {
            rc_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            rc_desc.Format = DXGI_FORMAT_UNKNOWN;
            rc_desc.MipLevels = 1;
            rc_desc.DepthOrArraySize = 1;
            rc_desc.Height = 1;
            rc_desc.Width = sizeof(ObjInfo) * CBCOUNT;
            rc_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
            rc_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            rc_desc.SampleDesc.Count = 1;
        }

        auto incre = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        for (auto i = 0u; i < FrameAmmount; i++) {
            res = device_->CreateCommittedResource
            (
                &hp_prop,
                D3D12_HEAP_FLAG_NONE,
                &rc_desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&SB[i].rsc_ptr)
            );
            if (FAILED(res)) return 0;

            res = SB[i].rsc_ptr->Map(0, nullptr, reinterpret_cast<void**>(&SB[i].view));
            memset(SB[i].view, 0, CBCOUNT * sizeof(ObjInfo));

            D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
            {
                srv.Format = DXGI_FORMAT_UNKNOWN;
                srv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srv.Buffer.FirstElement = 0;
                srv.Buffer.NumElements = CBCOUNT  ;
                srv.Buffer.StructureByteStride = sizeof(ObjInfo);
                srv.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
            }

            auto HCPU_ = heapCBV_SRV_UAV_->GetCPUDescriptorHandleForHeapStart();
            auto HGPU_ = heapCBV_SRV_UAV_->GetGPUDescriptorHandleForHeapStart();

            HCPU_.ptr = (incre * (i + 3)) + heapCBV_SRV_UAV_->GetCPUDescriptorHandleForHeapStart().ptr;
            HGPU_.ptr = (incre * (i + 3)) + heapCBV_SRV_UAV_->GetGPUDescriptorHandleForHeapStart().ptr;

            SB[i].HCPU = HCPU_;
            SB[i].HGPU = HGPU_;

            device_->CreateShaderResourceView
            (
                SB[i].rsc_ptr,
                &srv,
                SB[i].HCPU
            );
        }
    }
    //--------------*****
    {

        enum
        {
            CB =0,
            TEX,
            SB,
            AMMOUNT
        };

        D3D12_ROOT_PARAMETER r_param[AMMOUNT] = {};
        {
            r_param[CB].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param[CB].Descriptor.RegisterSpace = 0;
            r_param[CB].Descriptor.ShaderRegister = 0;
            r_param[CB].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_Tex = {};
        {
            range_Tex.BaseShaderRegister = 0;
            range_Tex.NumDescriptors = 1;
            range_Tex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_Tex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_Tex.RegisterSpace = 0;
        }

        r_param[TEX];
        {
            r_param[TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[TEX].DescriptorTable.NumDescriptorRanges = 1;
            r_param[TEX].DescriptorTable.pDescriptorRanges = &range_Tex;
            r_param[TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        }

        D3D12_DESCRIPTOR_RANGE range_SB = {};
        {
            range_SB.BaseShaderRegister = 1;
            range_SB.NumDescriptors = CBCOUNT;
            range_SB.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_SB.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_SB.RegisterSpace = 0;
        }

        r_param[SB];
        {
            r_param[SB].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[SB].DescriptorTable.NumDescriptorRanges = 1;
            r_param[SB].DescriptorTable.pDescriptorRanges = &range_SB;
            r_param[SB].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }
        

        D3D12_STATIC_SAMPLER_DESC sampler = {};
        {
            sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
            sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
            sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
            sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
            sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
            sampler.MaxAnisotropy = 1;
            sampler.MaxLOD = D3D12_FLOAT32_MAX;
            sampler.MinLOD = D3D12_FLOAT32_MAX * -1;
            sampler.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
            sampler.RegisterSpace = 0;
            sampler.ShaderRegister = 0;
            sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        }

        //--------++++++++++++++++++++++
        D3D12_ROOT_SIGNATURE_DESC r_s_desc = {};
        {
            auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

            r_s_desc.pParameters = r_param;
            r_s_desc.pStaticSamplers = &sampler;
            r_s_desc.Flags = flag;
            r_s_desc.NumParameters = AMMOUNT;
            r_s_desc.NumStaticSamplers = 1;
        }

        ID3DBlob* S_blob;
        ID3DBlob* E_blob;

        res = D3D12SerializeRootSignature
        (
            &r_s_desc,
            D3D_ROOT_SIGNATURE_VERSION_1_0,
            &S_blob,
            &E_blob
        );
        if (FAILED(res))     return 0;

        res = device_->CreateRootSignature
        (
            NULL,
            S_blob->GetBufferPointer(),
            S_blob->GetBufferSize(),
            IID_PPV_ARGS(&rootsig_)
        );
        if (FAILED(res))     return 0;
    }

    //----------------------*****
    view_.Height = Height;
    view_.Width = Width;
    view_.MaxDepth = 1.0f;
    view_.MinDepth = 0.0f;
    view_.TopLeftX = 0.0f;
    view_.TopLeftY = 0.0f;


    rect_.left = 0.0f;
    rect_.top = 0.0f;
    rect_.right = Width;
    rect_.bottom = Height;

    return true;
}

bool D3d::InitPSO()
{
    D3D12_INPUT_ELEMENT_DESC in_e_desc[2];
    {
        {
            in_e_desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
            in_e_desc[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
            in_e_desc[0].InputSlot = 0;
            in_e_desc[0].SemanticName = SEMANTICS_STR::POSITION;
            in_e_desc[0].SemanticIndex = 0;
            in_e_desc[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            in_e_desc[0].InstanceDataStepRate = 0;
        }
        {
            in_e_desc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
            in_e_desc[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
            in_e_desc[1].InputSlot = 0;
            in_e_desc[1].SemanticName = SEMANTICS_STR::TEXCOORD;
            in_e_desc[1].SemanticIndex = 0;
            in_e_desc[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            in_e_desc[1].InstanceDataStepRate = 0;
        }
    }

    D3D12_RASTERIZER_DESC rs_desc = {};
    {
        rs_desc.FillMode = D3D12_FILL_MODE_SOLID;
        rs_desc.CullMode = D3D12_CULL_MODE_BACK;
        rs_desc.FrontCounterClockwise = false;
        rs_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        rs_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        rs_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        rs_desc.AntialiasedLineEnable = false;
        rs_desc.DepthClipEnable = false;
        rs_desc.MultisampleEnable = false;
        rs_desc.ForcedSampleCount = 0;
        rs_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    }

    D3D12_RENDER_TARGET_BLEND_DESC rtb_desc =
    {
        false,false,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,D3D12_COLOR_WRITE_ENABLE_ALL
    };

    D3D12_BLEND_DESC bs_desc = {};
    {
        bs_desc.AlphaToCoverageEnable = false;
        bs_desc.IndependentBlendEnable = false;
        for (auto i = 0u; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {

            bs_desc.RenderTarget[i] = rtb_desc;
        }
    };

    D3D12_DEPTH_STENCIL_DESC dss_desc = {};
    {
        dss_desc.DepthEnable = 1;
        dss_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        dss_desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        dss_desc.StencilEnable = 0;
    }

    //----------------------

    ID3DBlob* VSblob = nullptr;
    auto res = D3DReadFileToBlob(SHADER_FILENAME::PoorVS, &VSblob);
    if (FAILED(res))     return 0;

    ID3DBlob* PSblob = nullptr;
    res = D3DReadFileToBlob(SHADER_FILENAME::PoorPS, &PSblob);
    if (FAILED(res))     return 0;
    //--------------------------

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
    {
        pso_desc.InputLayout = VERTEX::inp_Layout;
        pso_desc.pRootSignature = rootsig_;
        pso_desc.VS.pShaderBytecode = VSblob->GetBufferPointer();
        pso_desc.PS.pShaderBytecode = PSblob->GetBufferPointer();
        pso_desc.VS.BytecodeLength = VSblob->GetBufferSize();
        pso_desc.PS.BytecodeLength = PSblob->GetBufferSize();
        pso_desc.RasterizerState = rs_desc;
        pso_desc.BlendState = bs_desc;
        pso_desc.SampleDesc.Count = 1;
        pso_desc.SampleDesc.Quality = 0;
        pso_desc.SampleMask = UINT_MAX;
        pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        pso_desc.DepthStencilState = dss_desc;
        pso_desc.NumRenderTargets = 1;
        pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    }

    res = device_->CreateGraphicsPipelineState
    (
        &pso_desc,
        IID_PPV_ARGS(&PSO)
    );
    if (FAILED(res)) return 0;

    return true;
}

void D3d::Termination()
{
    TermGBO();
    waitGPU();
    if (event_fence != nullptr)
    {
        CloseHandle(event_fence);
        event_fence = nullptr;
    }

    SAFE_RELEASE(fence_);
    SAFE_RELEASE(heapRTV_);
    for (int i = 0; i < FrameAmmount; i++) {
        SAFE_RELEASE(colbuf_[i]);
        SAFE_RELEASE(cmdalloc_[i]);
    }
    SAFE_RELEASE(cmdlist_);
    SAFE_RELEASE(cmdque_);
    SAFE_RELEASE(swpchain_);

    SAFE_RELEASE(device_);


}

void D3d::TermGBO()
{
}

void D3d::Run(int interval)
{
    Update();
    write();
    //waitGPU();
    {
        brr.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        brr.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        brr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        brr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        brr.Transition.pResource = colbuf_[IND_frame];
        brr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    }

    cmdlist_->ResourceBarrier(1, &brr);
    cmdlist_->Close();


    ID3D12CommandList* commands[] = { cmdlist_ };
    cmdque_->ExecuteCommandLists(1, commands);
    present(interval);

}

void D3d::Update()
{
    {
        static float time_ = 0.0f;
        constexpr float incre = 1.0f / 60.0f;

        time_ += incre;
        CBV[IND_frame].ptr->time = time_;

        CBV[IND_frame].ptr->view = XMMatrixLookAtRH(CAM::Pos, CAM::Tgt, CAM::Head);
        CBV[IND_frame].ptr->proj = XMMatrixPerspectiveFovRH(CAM::Fov, CAM::Aspect, 1.0f, 1000.0f);
    }

    CAM::Run();

    auto y = sizeof(ObjInfo);
    
}

void D3d::SetHeight(float h)
{
    Height = h;
}

void D3d::SetWidth(float w)
{
    Width = w;
}

float D3d::GetHeight()
{
    return Height;
}

float D3d::GetWidth()
{
    return Width;
}

void D3d::SetColorBG(float R, float G, float B, float A)
{
    backcolor_[0] = R;
    backcolor_[1] = G;
    backcolor_[2] = B;
    backcolor_[3] = A;
}

ID3D12Device* D3d::GetDevice()
{
    return device_;
}

ID3D12GraphicsCommandList* D3d::GetCMDList()
{
    return cmdlist_;
}

D3d::~D3d()
{
}

D3d::D3d()
{
    Height = 0.0f;
    Width = 0.0f;
}

void D3d::write()
{
    cmdalloc_[IND_frame]->Reset();
    cmdlist_->Reset(cmdalloc_[IND_frame], nullptr);

    brr = {};
    {
        brr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        brr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        brr.Transition.pResource = colbuf_[IND_frame];
        brr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        brr.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        brr.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    }
    cmdlist_->ResourceBarrier(1, &brr);

    cmdlist_->OMSetRenderTargets(1, &h_RTV[IND_frame], FALSE, &h_ZBV);
    cmdlist_->ClearRenderTargetView(h_RTV[IND_frame], backcolor_, 0, nullptr);
    cmdlist_->ClearDepthStencilView(h_ZBV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    for (auto &itr : ResourceManager::models_) {
        auto v = 0u;

        for (auto cnt : itr.Mesh_) {

            memcpy(SB[IND_frame].view, itr.info.data(), sizeof(ObjInfo) * itr.info.size());

            cmdlist_->SetGraphicsRootSignature(rootsig_);
            cmdlist_->SetDescriptorHeaps(1, &heapCBV_SRV_UAV_);
            cmdlist_->SetGraphicsRootConstantBufferView(0, CBV[IND_frame].desc.BufferLocation);

            cmdlist_->SetGraphicsRootDescriptorTable(1, tex.HGPU);
            cmdlist_->SetGraphicsRootDescriptorTable(2, SB[IND_frame].HGPU);

            cmdlist_->SetPipelineState(PSO);

            cmdlist_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            cmdlist_->RSSetViewports(1, &view_);
            cmdlist_->RSSetScissorRects(1, &rect_);

            cmdlist_->SetGraphicsRootConstantBufferView(0, CBV[IND_frame].desc.BufferLocation);

            cmdlist_->IASetVertexBuffers(0, 1, &itr.VBV[v]);
            cmdlist_->IASetIndexBuffer(&itr.IBV[v]);
            cmdlist_->DrawIndexedInstanced(cnt.indexes_.size(), itr.DrawCount_, 0, 0, 0);
            
            v++;
        }
        S_Draw::Flush(&itr);
    }
}

void D3d::waitGPU()
{
    assert(cmdque_ != nullptr);
    assert(fence_ != nullptr);
    assert(event_fence != nullptr);

    cmdque_->Signal(fence_, fencecnt_[IND_frame]);

    fence_->SetEventOnCompletion(fencecnt_[IND_frame], event_fence);

    WaitForSingleObjectEx(event_fence, INFINITE, FALSE);

    fencecnt_[IND_frame]++;
}

void D3d::present(uint32_t itv)
{
    if (FAILED(swpchain_->Present(itv, 0))) return;

    const auto curval = fencecnt_[IND_frame];
    cmdque_->Signal(fence_, curval);

    IND_frame = swpchain_->GetCurrentBackBufferIndex();

    if (fence_->GetCompletedValue() < fencecnt_[IND_frame])
    {
        fence_->SetEventOnCompletion(fencecnt_[IND_frame], event_fence);
        WaitForSingleObjectEx(event_fence, INFINITE, FALSE);
    }

    fencecnt_[IND_frame] = curval + 1;
}

namespace PTR_D3D
{
    D3d* ptr = nullptr;
}

namespace PTR_WND
{
    HWND* ptr = nullptr;
}