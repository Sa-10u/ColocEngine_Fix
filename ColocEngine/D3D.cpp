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
constexpr UINT HPSIZE = 10;
constexpr UINT POST_HPSIZE = 1;

bool D3d::Initialize(HWND hwnd, uint32_t h, uint32_t w)
{
    PTR_D3D::ptr = this;
    PTR_WND::ptr = &hwnd;

    backcolor_[0] = .0f;
    backcolor_[1] = .0f;
    backcolor_[2] = .5f;
    backcolor_[3] = 1.0f;

    for (auto i = 0u; i < FrameAmount; ++i) {

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
        desc.BufferCount = FrameAmount;
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


    for (int i = 0u; i < FrameAmount; ++i) {

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
        hpdesc.NumDescriptors = FrameAmount;
        hpdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        hpdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        hpdesc.NodeMask = 0;
    }
    res = device_->CreateDescriptorHeap(&hpdesc, IID_PPV_ARGS((&heapRTV_)));
    if (FAILED(res))    return FAIL;

    D3D12_CPU_DESCRIPTOR_HANDLE handle = heapRTV_->GetCPUDescriptorHandleForHeapStart();
    UINT incre = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    for (UINT i = 0u; i < FrameAmount; ++i) {

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
        for (auto i = 0u; i < FrameAmount; ++i) {
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

    __CREATE("Buffer for Post Effect")
    {
        if (!InitPost())      return 0;
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
                hp_desc.NumDescriptors = (HPSIZE * FrameAmount) + ResourceManager::MAX_Textures;
                hp_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            }

            res = device_->CreateDescriptorHeap
            (
                &hp_desc,
                IID_PPV_ARGS(&ResourceManager::heapCBV_SRV_UAV_)
            );

            ResourceManager::DHH_CbSrUaV = new DH(device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), &ResourceManager::heapCBV_SRV_UAV_);

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

        for (auto i = 0; i < FrameAmount; ++i) {

            res = device_->CreateCommittedResource
            (
                &hp_proc_c,
                D3D12_HEAP_FLAG_NONE,
                &rc_desc_c,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&CB_Util[i])
            );
            if (FAILED(res))     return false;
            //----------------------------------

            auto address = CB_Util[i]->GetGPUVirtualAddress();

            CBV_Util[i].HCPU = ResourceManager::DHH_CbSrUaV->GetAndIncreCPU();
            CBV_Util[i].HGPU = ResourceManager::DHH_CbSrUaV->GetAndIncreGPU();;
            CBV_Util[i].desc.BufferLocation = address;
            CBV_Util[i].desc.SizeInBytes = sizeof(Util);

            device_->CreateConstantBufferView(&CBV_Util[i].desc, CBV_Util[i].HCPU);

            res = CB_Util[i]->Map(0, NULL, reinterpret_cast<void**>(&CBV_Util[i].ptr));
            if (FAILED(res)) return 0;
        }
    }
    //------------------------------------------------------------------

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
            rc_desc_c.Width = sizeof(Cam);
            rc_desc_c.DepthOrArraySize = 1;
            rc_desc_c.Flags = D3D12_RESOURCE_FLAG_NONE;
            rc_desc_c.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

            rc_desc_c.SampleDesc.Count = 1;
            rc_desc_c.SampleDesc.Quality = 0;
        }

        for (auto i = 0; i < FrameAmount; ++i) {

            res = device_->CreateCommittedResource
            (
                &hp_proc_c,
                D3D12_HEAP_FLAG_NONE,
                &rc_desc_c,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&CB_CAM[i])
            );
            if (FAILED(res))     return false;
            //----------------------------------

            auto address = CB_CAM[i]->GetGPUVirtualAddress();

            CBV_Cam[i].HCPU = ResourceManager::DHH_CbSrUaV->GetAndIncreCPU();
            CBV_Cam[i].HGPU = ResourceManager::DHH_CbSrUaV->GetAndIncreGPU();;
            CBV_Cam[i].desc.BufferLocation = address;
            CBV_Cam[i].desc.SizeInBytes = sizeof(Cam);

            device_->CreateConstantBufferView(&CBV_Cam[i].desc, CBV_Cam[i].HCPU);

            res = CB_CAM[i]->Map(0, NULL, reinterpret_cast<void**>(&CBV_Cam[i].ptr));
            if (FAILED(res)) return 0;
        }
    }
    //----------------------------
    {
        D3D12_HEAP_PROPERTIES hp_prop = {};
        {
            hp_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            hp_prop.CreationNodeMask = 1;
            hp_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            hp_prop.Type = D3D12_HEAP_TYPE_UPLOAD;
            hp_prop.VisibleNodeMask = 1;
        }

        D3D12_RESOURCE_DESC rsc_desc = {};
        {
            rsc_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            rsc_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
            rsc_desc.Format = DXGI_FORMAT_UNKNOWN;
            rsc_desc.MipLevels = 1;
            rsc_desc.Alignment = 0;
            rsc_desc.DepthOrArraySize = 1;
            rsc_desc.Height = 1;
            rsc_desc.Width =
                (
                    LightManager::lights.amb.GetSize() +
                    LightManager::lights.dir.GetSize() +
                    LightManager::lights.point.GetSize()
                );
            rsc_desc.SampleDesc.Count = 1;
            rsc_desc.SampleDesc.Quality = 0;
            rsc_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        }
        
        for (auto i = 0u; i < FrameAmount; i++) {

            res = device_->CreateCommittedResource
            (
                &hp_prop,
                D3D12_HEAP_FLAG_NONE,
                &rsc_desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&CB_LGT[i])
            );
            if (FAILED(res)) return 0;

            CBV_LGT[i].ptr;

            CBV_LGT[i].desc.BufferLocation = CB_LGT[i]->GetGPUVirtualAddress();
            CBV_LGT[i].desc.SizeInBytes = rsc_desc.Width;
            CBV_LGT[i].HCPU = ResourceManager::DHH_CbSrUaV->GetAndIncreCPU();
            CBV_LGT[i].HGPU = ResourceManager::DHH_CbSrUaV->GetAndIncreGPU();

            device_->CreateConstantBufferView
            (
                &CBV_LGT[i].desc,
                CBV_LGT[i].HCPU
            );

            void* ptr = nullptr;
            res = CB_LGT[i]->Map(0, NULL, reinterpret_cast<void**>(&CBV_LGT[i].ptr));
            if (FAILED(res)) return 0;
        }
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

        for (auto i = 0u; i < FrameAmount; i++) {
            res = device_->CreateCommittedResource
            (
                &hp_prop,
                D3D12_HEAP_FLAG_NONE,
                &rc_desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&SB_OI[i].rsc_ptr)
            );
            if (FAILED(res)) return 0;

            res = SB_OI[i].rsc_ptr->Map(0, nullptr, reinterpret_cast<void**>(&SB_OI[i].view));
            memset(SB_OI[i].view, 0, CBCOUNT * sizeof(ObjInfo));

            D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
            {
                srv.Format = DXGI_FORMAT_UNKNOWN;
                srv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srv.Buffer.FirstElement = 0;
                srv.Buffer.NumElements = CBCOUNT;
                srv.Buffer.StructureByteStride = sizeof(ObjInfo);
                srv.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
            }

            SB_OI[i].HCPU = ResourceManager::DHH_CbSrUaV->GetAndIncreCPU();
            SB_OI[i].HGPU = ResourceManager::DHH_CbSrUaV->GetAndIncreGPU();

            device_->CreateShaderResourceView
            (
                SB_OI[i].rsc_ptr,
                &srv,
                SB_OI[i].HCPU
            );
        }
    }

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
            rc_desc.Width = sizeof(MapBOOL) * CBCOUNT;
            rc_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
            rc_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            rc_desc.SampleDesc.Count = 1;
        }

        for (auto i = 0u; i < FrameAmount; i++) {
            res = device_->CreateCommittedResource
            (
                &hp_prop,
                D3D12_HEAP_FLAG_NONE,
                &rc_desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&SB_MB[i].rsc_ptr)
            );
            if (FAILED(res)) return 0;

            res = SB_MB[i].rsc_ptr->Map(0, nullptr, reinterpret_cast<void**>(&SB_MB[i].view));
            memset(SB_MB[i].view, 0, CBCOUNT * sizeof(MapBOOL));

            D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
            {
                srv.Format = DXGI_FORMAT_UNKNOWN;
                srv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srv.Buffer.FirstElement = 0;
                srv.Buffer.NumElements = CBCOUNT;
                srv.Buffer.StructureByteStride = sizeof(MapBOOL);
                srv.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
            }

            SB_MB[i].HCPU = ResourceManager::DHH_CbSrUaV->GetAndIncreCPU();
            SB_MB[i].HGPU = ResourceManager::DHH_CbSrUaV->GetAndIncreGPU();

            device_->CreateShaderResourceView
            (
                SB_MB[i].rsc_ptr,
                &srv,
                SB_MB[i].HCPU
            );
        }
    }

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
            rc_desc.Width = sizeof(Material) * CBCOUNT;
            rc_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
            rc_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            rc_desc.SampleDesc.Count = 1;
        }

        for (auto i = 0u; i < FrameAmount; i++) {
            res = device_->CreateCommittedResource
            (
                &hp_prop,
                D3D12_HEAP_FLAG_NONE,
                &rc_desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&SB_MTL[i].rsc_ptr)
            );
            if (FAILED(res)) return 0;

            res = SB_MTL[i].rsc_ptr->Map(0, nullptr, reinterpret_cast<void**>(&SB_MTL[i].view));
            memset(SB_MTL[i].view, 0, CBCOUNT * sizeof(Material));

            D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
            {
                srv.Format = DXGI_FORMAT_UNKNOWN;
                srv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srv.Buffer.FirstElement = 0;
                srv.Buffer.NumElements = CBCOUNT;
                srv.Buffer.StructureByteStride = sizeof(Material);
                srv.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
            }

            SB_MTL[i].HCPU = ResourceManager::DHH_CbSrUaV->GetAndIncreCPU();
            SB_MTL[i].HGPU = ResourceManager::DHH_CbSrUaV->GetAndIncreGPU();

            device_->CreateShaderResourceView
            (
                SB_MTL[i].rsc_ptr,
                &srv,
                SB_MTL[i].HCPU
            );
        }
    }
    //--------------*****

    {

        enum
        {
            CB_U = 0,
            CB_C,
            CB_L,
            SB_MTL,
            SB_OI,
            SB_MB,
            TEX,

            Amount
        };

        D3D12_ROOT_PARAMETER r_param[Amount] = {};
        {
            r_param[CB_U].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param[CB_U].Descriptor.RegisterSpace = 0;
            r_param[CB_U].Descriptor.ShaderRegister = 0;
            r_param[CB_U].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        {
            r_param[CB_C].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param[CB_C].Descriptor.RegisterSpace = 0;
            r_param[CB_C].Descriptor.ShaderRegister = 256;
            r_param[CB_C].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        {
            r_param[CB_L].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param[CB_L].Descriptor.RegisterSpace = 0;
            r_param[CB_L].Descriptor.ShaderRegister = 512;
            r_param[CB_L].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_SBOI = {};
        {
            range_SBOI.BaseShaderRegister = 0;
            range_SBOI.NumDescriptors = CBCOUNT;
            range_SBOI.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_SBOI.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_SBOI.RegisterSpace = 0;
        }

        r_param[SB_OI];
        {
            r_param[SB_OI].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[SB_OI].DescriptorTable.NumDescriptorRanges = 1;
            r_param[SB_OI].DescriptorTable.pDescriptorRanges = &range_SBOI;
            r_param[SB_OI].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_SBMB = {};
        {
            range_SBMB.BaseShaderRegister = 512;
            range_SBMB.NumDescriptors = CBCOUNT;
            range_SBMB.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_SBMB.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_SBMB.RegisterSpace = 0;
        }

        r_param[SB_MB];
        {
            r_param[SB_MB].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[SB_MB].DescriptorTable.NumDescriptorRanges = 1;
            r_param[SB_MB].DescriptorTable.pDescriptorRanges = &range_SBMB;
            r_param[SB_MB].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_SBMTL = {};
        {
            range_SBMTL.BaseShaderRegister = 1024;
            range_SBMTL.NumDescriptors = CBCOUNT;
            range_SBMTL.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_SBMTL.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_SBMTL.RegisterSpace = 0;
        }

        r_param[SB_MTL];
        {
            r_param[SB_MTL].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[SB_MTL].DescriptorTable.NumDescriptorRanges = 1;
            r_param[SB_MTL].DescriptorTable.pDescriptorRanges = &range_SBMTL;
            r_param[SB_MTL].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_Tex = {};
        {
            range_Tex.BaseShaderRegister = 1792;
            range_Tex.NumDescriptors = -1;
            range_Tex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_Tex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_Tex.RegisterSpace = 0;
        }

        r_param[TEX];
        {
            r_param[TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[TEX].DescriptorTable.NumDescriptorRanges = 1;
            r_param[TEX].DescriptorTable.pDescriptorRanges = &range_Tex;
            r_param[TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
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
            r_s_desc.NumParameters = Amount;
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
    D3D12_RASTERIZER_DESC rs_desc = {};
    {
        rs_desc.FillMode = D3D12_FILL_MODE_SOLID;
        rs_desc.CullMode = D3D12_CULL_MODE_FRONT;
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
        true,false,
        D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
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
    auto res = D3DReadFileToBlob(SHADER_FILENAME::DefVS, &VSblob);
    if (FAILED(res))     return 0;

    ID3DBlob* PSblob = nullptr;
    res = D3DReadFileToBlob(SHADER_FILENAME::DefPS, &PSblob);
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

bool D3d::InitPost()
{
    HRESULT res = {};
    auto desc_hp = heapRTV_->GetDesc();
    auto desc_rsc = colbuf_[0]->GetDesc();

    {
        D3D12_HEAP_PROPERTIES prop_hp = {};
        {
            prop_hp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            prop_hp.CreationNodeMask = 1;
            prop_hp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            prop_hp.Type = D3D12_HEAP_TYPE_DEFAULT;
            prop_hp.VisibleNodeMask = 1;
        }
        D3D12_CLEAR_VALUE val = {};
        {
            val.DepthStencil.Depth = 1.0f;
            val.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

            val.Color[0] = backcolor_[0];
            val.Color[1] = backcolor_[1];
            val.Color[2] = backcolor_[2];
            val.Color[3] = backcolor_[3];
        }

        res = device_->CreateCommittedResource
        (
            &prop_hp,
            D3D12_HEAP_FLAG_NONE,
            &desc_rsc,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            &val,
            IID_PPV_ARGS(&post_)
        );
        if (FAILED(res)) return false;
    }
    //-----------------------------------------

    {
        desc_hp.NumDescriptors = 1;

        res = device_->CreateDescriptorHeap
        (
            &desc_hp,
            IID_PPV_ARGS(&postRTV_)
        );
        if (FAILED(res)) return false;

        D3D12_RENDER_TARGET_VIEW_DESC desc_prtv = {};
        {
            desc_prtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            desc_prtv.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        }

        device_->CreateRenderTargetView
        (
            post_,
            &desc_prtv,
            postRTV_->GetCPUDescriptorHandleForHeapStart()
        );
        //-------------------------------------------------------------

        {
            desc_hp.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            desc_hp.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

            res = device_->CreateDescriptorHeap
            (
                &desc_hp,
                IID_PPV_ARGS(&postSRV_)
            );
            if (FAILED(res)) return false;

            D3D12_SHADER_RESOURCE_VIEW_DESC desc_psrv = {};
            {
                desc_psrv.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
                desc_psrv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                desc_psrv.Texture2D.MipLevels = 1;
                desc_psrv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            }

            device_->CreateShaderResourceView
            (
                post_,
                &desc_psrv,
                postSRV_->GetCPUDescriptorHandleForHeapStart()
            );
        }
    }
    //----------------------------------------------
    const UINT QUAD = 4;
    SIMPLEVERTEX vxs[QUAD] = {};
    {
        float z = 0.1;
        vxs[0].pos = { -1,-1,z };
        vxs[0].uv = { 0,1 };

        vxs[1].pos = { -1,1,z };
        vxs[1].uv = { 0,0 };

        vxs[2].pos = { 1,-1,z };
        vxs[2].uv = { 1,1 };

        vxs[3].pos = { 1,1,z };
        vxs[3].uv = { 1,0 };
    }

    D3D12_HEAP_PROPERTIES hp_prop_v = {};
    {
        hp_prop_v.Type = D3D12_HEAP_TYPE_UPLOAD;
        hp_prop_v.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        hp_prop_v.CreationNodeMask = 1;
        hp_prop_v.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        hp_prop_v.VisibleNodeMask = 1;
    };
    D3D12_RESOURCE_DESC rc_desc_v = {};
    {
        rc_desc_v.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        rc_desc_v.Alignment = 0;
        rc_desc_v.Width = sizeof(vxs);
        rc_desc_v.Height = 1;
        rc_desc_v.DepthOrArraySize = 1;
        rc_desc_v.MipLevels = 1;
        rc_desc_v.Format = DXGI_FORMAT_UNKNOWN;
        rc_desc_v.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        rc_desc_v.Flags = D3D12_RESOURCE_FLAG_NONE;

        rc_desc_v.SampleDesc.Count = 1;
        rc_desc_v.SampleDesc.Quality = 0;
    }

    res = device_->CreateCommittedResource
    (
        &hp_prop_v,
        D3D12_HEAP_FLAG_NONE,
        &rc_desc_v,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&postVB_)
    );
    if (FAILED(res)) return false;

    void* ptr = nullptr;
    res = postVB_->Map(0, nullptr, &ptr);
    if (FAILED(res)) return false;

    memcpy(ptr, vxs, sizeof(vxs));
    postVB_->Unmap(0, nullptr);

    postVBV_.BufferLocation = postVB_->GetGPUVirtualAddress();
    postVBV_.SizeInBytes = sizeof(vxs);
    postVBV_.StrideInBytes = sizeof(vxs[0]);//datnum type of SIMPLEVERTEX;

    //-------------------------------------------------------------

    D3D12_RASTERIZER_DESC rs_desc = {};
    {
        rs_desc.FillMode = D3D12_FILL_MODE_SOLID;
        rs_desc.CullMode = D3D12_CULL_MODE_FRONT;
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
    }

    std::wstring file_str = {};

    ID3DBlob* VSblob = nullptr;
    FileLoad(L"PostEffect_Default_VS.cso", &file_str);
    res = D3DReadFileToBlob(file_str.c_str(), &VSblob);
    if (FAILED(res))     return 0;

    ID3DBlob* PSblob = nullptr;
    FileLoad(L"PostEffect_Default_PS.cso", &file_str);
    res = D3DReadFileToBlob(file_str.c_str(), &PSblob);
    if (FAILED(res))     return 0;

    //-------------------------
    D3D12_ROOT_SIGNATURE_DESC rootsig = {};
    {
        auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        rootsig.Flags = flag;
        rootsig.NumParameters = 0;
        rootsig.NumStaticSamplers = 0;
    }

    ID3DBlob* Sblob = nullptr;
    ID3DBlob* Eblob = nullptr;

    res = D3D12SerializeRootSignature
    (
        &rootsig,
        D3D_ROOT_SIGNATURE_VERSION_1_0,
        &Sblob,
        &Eblob
    );
    if (FAILED(res)) return false;

    res = device_->CreateRootSignature
    (
        0,
        Sblob->GetBufferPointer(),
        Sblob->GetBufferSize(),
        IID_PPV_ARGS(&postRTSG_)
    );
    if (FAILED(res)) return false;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gps_desc = {};
    {
        gps_desc.InputLayout = SIMPLEVERTEX::inp_Layout;
        gps_desc.BlendState = bs_desc;
        gps_desc.NumRenderTargets = 1;
        gps_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        gps_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        gps_desc.RasterizerState = rs_desc;
        gps_desc.PS.BytecodeLength = PSblob->GetBufferSize();
        gps_desc.PS.pShaderBytecode = PSblob->GetBufferPointer();
        gps_desc.VS.BytecodeLength = VSblob->GetBufferSize();
        gps_desc.VS.pShaderBytecode = VSblob->GetBufferPointer();
        gps_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
        gps_desc.SampleMask = UINT_MAX;
        gps_desc.SampleDesc.Count = 1;
        gps_desc.SampleDesc.Quality = 0;
        gps_desc.pRootSignature = postRTSG_;
    }

    res = device_->CreateGraphicsPipelineState
    (
        &gps_desc,
        IID_PPV_ARGS(&postPSO)
    );
    if (FAILED(res)) return false;

    //-----------------------------------

    D3D12_DESCRIPTOR_HEAP_DESC hpd_desc = {};
    {
        hpd_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        hpd_desc.NodeMask = 0;
        hpd_desc.NumDescriptors = FrameAmount * POST_HPSIZE;
        hpd_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    }

    res = device_->CreateDescriptorHeap
    (
        &hpd_desc,
        IID_PPV_ARGS(&ResourceManager::postCBV_SRV_UAV_)
    );
    ResourceManager::DHPost_CbSrUaV = new DH(device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), &ResourceManager::postCBV_SRV_UAV_);

    if (FAILED(res)) return false;


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
    for (int i = 0; i < FrameAmount; i++) {
        SAFE_RELEASE(colbuf_[i]);
        SAFE_RELEASE(cmdalloc_[i]);
    }
    SAFE_RELEASE(cmdlist_);
    SAFE_RELEASE(cmdque_);
    SAFE_RELEASE(swpchain_);

    SAFE_RELEASE(device_);
    delete ResourceManager::DHH_CbSrUaV;
    delete ResourceManager::DHPost_CbSrUaV;

}

void D3d::TermGBO()
{
}

void D3d::Run(int interval)
{
    Update();
    write();
    //postEffect();
    render();
    present(0);

}

void D3d::Update()
{
    {
        static float time_ = 0.0f;
        constexpr float incre = 1.0f / 60.0f;

        time_ += incre;
        CBV_Util[IND_frame].ptr->time = time_;
        CBV_Util[IND_frame].ptr->view = XMMatrixLookAtRH(CAM::Pos, CAM::Tgt, CAM::Head);
        CBV_Util[IND_frame].ptr->proj = XMMatrixPerspectiveFovRH(CAM::Fov, CAM::Aspect, 1.0f, 1000.0f);

        XMStoreFloat3(&CBV_Cam[IND_frame].ptr->pos, CAM::Pos);
        XMStoreFloat3(&CBV_Cam[IND_frame].ptr->tgt, CAM::Tgt);
        
        memcpy(&CBV_LGT[IND_frame].ptr->amb, LightManager::lights.amb.pLight(), LightManager::lights.amb.GetSize());
        memcpy(&CBV_LGT[IND_frame].ptr->dir, LightManager::lights.dir.pLight(), LightManager::lights.dir.GetSize());
        memcpy(&CBV_LGT[IND_frame].ptr->point, LightManager::lights.point.pLight() , LightManager::lights.point.GetSize());
    }

    CAM::Run();

    auto y = sizeof(ObjInfo);
    auto s3 = sizeof(LightManager::Lights);
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
    //auto handle = postRTV_->GetCPUDescriptorHandleForHeapStart();
    auto handle = h_RTV[IND_frame];

    enum RP
    {
        CBU = 0,
        CBC,
        CBL,
        SBMTL,
        SBOI,
        SBMB,
        TEX,
        Amount
    };
    brr = {};
    {
        brr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        brr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        //brr.Transition.pResource = post_;
        brr.Transition.pResource = colbuf_[IND_frame];
        brr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        //brr.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        brr.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        brr.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    }
    cmdlist_->ResourceBarrier(1, &brr);

    cmdalloc_[IND_frame]->Reset();
    cmdlist_->Reset(cmdalloc_[IND_frame], nullptr);

    cmdlist_->OMSetRenderTargets(1, &handle, FALSE, &h_ZBV);
    cmdlist_->ClearRenderTargetView(handle, backcolor_, 0, nullptr);
    cmdlist_->ClearDepthStencilView(h_ZBV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    auto MDIND = 0u;
    for (auto& itr : ResourceManager::models_) {
        
            auto v = 0u;
            for (auto cnt : itr.Mesh_) {

                cmdlist_->OMSetRenderTargets(1, &handle, FALSE, &h_ZBV);

                cmdlist_->SetGraphicsRootSignature(rootsig_);

                cmdlist_->SetDescriptorHeaps(1, ResourceManager::DHH_CbSrUaV->ppHeap_);

                cmdlist_->SetGraphicsRootConstantBufferView(CBU, CBV_Util[IND_frame].desc.BufferLocation);
                cmdlist_->SetGraphicsRootConstantBufferView(CBC, CBV_Cam[IND_frame].desc.BufferLocation);
                cmdlist_->SetGraphicsRootConstantBufferView(CBL, CBV_LGT[IND_frame].desc.BufferLocation);

                cmdlist_->SetGraphicsRootDescriptorTable(TEX, ResourceManager::E_Tex.tex_.HGPU);

                cmdlist_->SetGraphicsRootDescriptorTable(SBMTL, SB_MTL[IND_frame].HGPU);
                cmdlist_->SetGraphicsRootDescriptorTable(SBOI, SB_OI[IND_frame].HGPU);
                cmdlist_->SetGraphicsRootDescriptorTable(SBMB, SB_MB[IND_frame].HGPU);

                cmdlist_->SetPipelineState(PSO);

                cmdlist_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                cmdlist_->RSSetViewports(1, &view_);
                cmdlist_->RSSetScissorRects(1, &rect_);

                cmdlist_->SetGraphicsRootConstantBufferView(0, CBV_Util[IND_frame].desc.BufferLocation);


                {
                    SB_MTL[IND_frame].view[v].alp = itr.Mtr_[v].alpha_;
                    SB_MTL[IND_frame].view[v].dif = itr.Mtr_[v].dif_;
                    SB_MTL[IND_frame].view[v].emis = itr.Mtr_[v].emis_;
                    SB_MTL[IND_frame].view[v].shin = itr.Mtr_[v].shin_;
                    SB_MTL[IND_frame].view[v].spec = itr.Mtr_[v].spec_;
                    SB_MTL[IND_frame].view[v].val0 = 0;
                }

                {
                    memcpy(SB_OI[IND_frame].view, itr.info.data(), sizeof(ObjInfo)* itr.info.size());
                    memcpy(SB_MB[IND_frame].view, itr.Mesh_[v].texIndex_.data(), sizeof(MapBOOL)* itr.Mesh_[v].texIndex_.size());
                }

                cmdlist_->IASetVertexBuffers(0, 1, &itr.VBV[v]);
                cmdlist_->IASetIndexBuffer(&itr.IBV[v]);
                if (v == 4) continue;//test
                cmdlist_->DrawIndexedInstanced(cnt.indexes_.size(), itr.DrawCount_, 0, 0, 0);

                v++;
                cmdlist_->Close();
                ID3D12CommandList* commands[] = { cmdlist_ };
                cmdque_->ExecuteCommandLists(1, commands);

                waitGPU();
                cmdalloc_[IND_frame]->Reset();
                cmdlist_->Reset(cmdalloc_[IND_frame], nullptr);

            }
            S_Draw::Flush(MDIND);
            MDIND++;
    }

    brr = {};
    {
        brr.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        brr.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        brr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        brr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        brr.Transition.pResource = post_;
        brr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    }
    //cmdlist_->ResourceBarrier(1, &brr);
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

void D3d::present(int itv)
{
    swpchain_->Present(itv, 0);

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

void D3d::postEffect()
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

    cmdlist_->OMSetRenderTargets(1, &h_RTV[IND_frame], FALSE, nullptr);
    cmdlist_->ClearRenderTargetView(h_RTV[IND_frame], backcolor_, 0, nullptr);

    cmdlist_->SetGraphicsRootSignature(postRTSG_);

    //cmdlist_->SetDescriptorHeaps(1, DHPost_CbSrUaV->ppHeap_);
    {
        //cmdlist_->SetGraphicsRootConstantBufferView(0, CBV_Util[IND_frame].desc.BufferLocation);
    }

    cmdlist_->SetPipelineState(postPSO);
    cmdlist_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    cmdlist_->RSSetViewports(1, &view_);
    cmdlist_->RSSetScissorRects(1, &rect_);

    cmdlist_->IASetVertexBuffers(0, 1, &postVBV_);
    cmdlist_->IASetIndexBuffer(&postIBV_);

    cmdlist_->DrawInstanced(4, 1, 0, 0);
}

void D3d::render()
{
    brr = {};
    {
        brr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        brr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        brr.Transition.pResource = colbuf_[IND_frame];
        brr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        brr.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        brr.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    }
    cmdlist_->ResourceBarrier(1, &brr);
    cmdlist_->Close();

    ID3D12CommandList* commands[] = { cmdlist_ };
    cmdque_->ExecuteCommandLists(1, commands);
}

namespace PTR_D3D
{
    D3d* ptr = nullptr;
}

namespace PTR_WND
{
    HWND* ptr = nullptr;
}