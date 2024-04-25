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
#include"PSOManager.h"

constexpr UINT HPSIZE = 10;
constexpr UINT POST_HPSIZE = 1;
constexpr UINT UI_HPSIZE = 1;

bool D3d::Initialize(HWND hwnd, uint32_t h, uint32_t w)
{
#if 1
    {
        ID3D12Debug* dbg = {};

        if (!FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&dbg))))
        {
            dbg->EnableDebugLayer();
        }
    }
#endif

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

    if(!PSOManager::Init())  return 0;

    __CREATE("Buffer for Post Effect")
    {
        if (!InitPost())      return false;
    }

    __CREATE("Heap for UI")
    {
        if (!InitUI())       return false;
    }

    __CREATE("Context for Text")
    {
        if (!InitText())       return false;
    }

    return true;
}

bool D3d::InitGBO()
{
    HRESULT&& res = FALSE;

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
                    P_Light::GetSize()+
                    D_Light::GetSize()+
                    A_Light::GetSize()
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
            rc_desc.Width = sizeof(ObjInfo) * ResourceManager::CBCOUNT;
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
            memset(SB_OI[i].view, 0, ResourceManager::CBCOUNT * sizeof(ObjInfo));

            D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
            {
                srv.Format = DXGI_FORMAT_UNKNOWN;
                srv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srv.Buffer.FirstElement = 0;
                srv.Buffer.NumElements = ResourceManager::CBCOUNT;
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
            rc_desc.Width = sizeof(MapBOOL) * ResourceManager::CBCOUNT;
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
            memset(SB_MB[i].view, 0, ResourceManager::CBCOUNT * sizeof(MapBOOL));

            D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
            {
                srv.Format = DXGI_FORMAT_UNKNOWN;
                srv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srv.Buffer.FirstElement = 0;
                srv.Buffer.NumElements = ResourceManager::CBCOUNT;
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
            rc_desc.Width = sizeof(Material) * ResourceManager::CBCOUNT;
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
            memset(SB_MTL[i].view, 0, ResourceManager::CBCOUNT * sizeof(Material));

            D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
            {
                srv.Format = DXGI_FORMAT_UNKNOWN;
                srv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srv.Buffer.FirstElement = 0;
                srv.Buffer.NumElements = ResourceManager::CBCOUNT;
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
                rc_desc.Width = sizeof(SimpleInfo_UI) * ResourceManager::CBCOUNT;
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
                    IID_PPV_ARGS(&SB_UI[i].rsc_ptr)
                );
                if (FAILED(res)) return 0;

                res = SB_UI[i].rsc_ptr->Map(0, nullptr, reinterpret_cast<void**>(&SB_UI[i].view));
                memset(SB_UI[i].view, 0, ResourceManager::CBCOUNT * sizeof(SimpleInfo_UI));

                D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
                {
                    srv.Format = DXGI_FORMAT_UNKNOWN;
                    srv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                    srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                    srv.Buffer.FirstElement = 0;
                    srv.Buffer.NumElements = ResourceManager::CBCOUNT;
                    srv.Buffer.StructureByteStride = sizeof(SimpleInfo_UI);
                    srv.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
                }

                D3D12_DESCRIPTOR_HEAP_DESC hpd_desc = {};
                {
                    hpd_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
                    hpd_desc.NodeMask = 0;
                    hpd_desc.NumDescriptors = FrameAmount * UI_HPSIZE;
                    hpd_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
                }

                device_->CreateDescriptorHeap
                (
                    &hpd_desc,
                    IID_PPV_ARGS(&ResourceManager::uiCBV_SRV_UAV_)
                );

                ResourceManager::DHUI_CbSrUaV = new DH(device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), &ResourceManager::uiCBV_SRV_UAV_);

                SB_UI[i].HCPU = ResourceManager::DHUI_CbSrUaV->GetAndIncreCPU();
                SB_UI[i].HGPU = ResourceManager::DHUI_CbSrUaV->GetAndIncreGPU();

                device_->CreateShaderResourceView
                (
                    SB_UI[i].rsc_ptr,
                    &srv,
                    SB_UI[i].HCPU
                );
            }
        }
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

bool D3d::InitPost()
{
    HRESULT&& res = NULL;
    auto desc_hp = heapRTV_->GetDesc();
    auto desc_rsc = colbuf_[0]->GetDesc();

    {

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
    SIMPLEVERTEX vxs[C_Quad::QUAD_VERTEX] = {};
    {
        float z = -1;
       
        vxs[0].pos = { -1,1,z };
        vxs[0].uv = { 0,0 };  
        
        vxs[1].pos = { 1,1,z };
        vxs[1].uv = { 1,0 };

        vxs[2].pos = { 1,-1,z };
        vxs[2].uv = { 1,1 };

        vxs[3].pos = { -1,-1,z };
        vxs[3].uv = { 0,1 };
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
        IID_PPV_ARGS(&quadVB_)
    );
    if (FAILED(res)) return false;

    void* ptr = nullptr;
    res = quadVB_->Map(0, nullptr, &ptr);
    if (FAILED(res)) return false;

    memcpy(ptr, vxs, sizeof(vxs));
    quadVB_->Unmap(0, nullptr);

    quadVBV_.BufferLocation = quadVB_->GetGPUVirtualAddress();
    quadVBV_.SizeInBytes = sizeof(vxs);
    quadVBV_.StrideInBytes = sizeof(vxs[0]);//datnum type of SIMPLEVERTEX;

    return true;
}

bool D3d::InitUI()
{
    HRESULT res = {};
    {
        D3D12_DESCRIPTOR_HEAP_DESC hp_desc = {};
        {
            hp_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            hp_desc.NodeMask = 1;
            hp_desc.NumDescriptors = ResourceManager::CBCOUNT;
            hp_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        }

        res = device_->CreateDescriptorHeap
        (
            &hp_desc,
            IID_PPV_ARGS(&ResourceManager::uiCBV_SRV_UAV_)
        );
        if (FAILED(res)) return false;

        ResourceManager::DHUI_CbSrUaV = new DH(device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), &ResourceManager::uiCBV_SRV_UAV_);
    }

    return true;
}

bool D3d::InitText()
{
    HRESULT&& res = NULL;

    res = DWriteCreateFactory
    (
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&dwfac_)
    );
    if (FAILED(res)) return false;

    {
        ID3D11Device* dev_;

        res = D3D11On12CreateDevice
        (
            device_,
            D3D11_CREATE_DEVICE_
        );
    }

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
    delete ResourceManager::DHUI_CbSrUaV;
}

void D3d::TermGBO()
{
}

void D3d::Run(int interval)
{
    Update();
    write();
    preeffectUI();
    //postEffect();
    constantUI();
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

        CBV_Util[IND_frame].ptr->PL_CNT = P_Light::GetCount();
        CBV_Util[IND_frame].ptr->DL_CNT = D_Light::GetCount();
        CBV_Util[IND_frame].ptr->AL_CNT = A_Light::GetCount();

        XMStoreFloat3(&CBV_Cam[IND_frame].ptr->pos, CAM::Pos);
        XMStoreFloat3(&CBV_Cam[IND_frame].ptr->tgt, CAM::Tgt);
        
        memcpy(&CBV_LGT[IND_frame].ptr->pl, P_Light::pLight(), P_Light::GetSize());
        memcpy(&CBV_LGT[IND_frame].ptr->dl, D_Light::pLight(), D_Light::GetSize());
        memcpy(&CBV_LGT[IND_frame].ptr->al, A_Light::pLight(), A_Light::GetSize());
    }

    CAM::Run();
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
    //static auto handle = postRTV_->GetCPUDescriptorHandleForHeapStart();
    auto handle = h_RTV[IND_frame];

    
    brr = {};
    {
        brr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        brr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        brr.Transition.pResource = post_;
        //brr.Transition.pResource = colbuf_[IND_frame];

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
            for (auto& cnt : itr.Mesh_) {

                cmdlist_->OMSetRenderTargets(1, &handle, FALSE, &h_ZBV);
                cmdlist_->SetGraphicsRootSignature(PSOManager::GetPSO(PSOManager::Shader3D::Default)->GetRTSG());

                cmdlist_->SetDescriptorHeaps(1, ResourceManager::DHH_CbSrUaV->ppHeap_);

                cmdlist_->SetGraphicsRootConstantBufferView(PSOManager::CB_U, CBV_Util[IND_frame].desc.BufferLocation);
                cmdlist_->SetGraphicsRootConstantBufferView(PSOManager::CB_C, CBV_Cam[IND_frame].desc.BufferLocation);
                cmdlist_->SetGraphicsRootConstantBufferView(PSOManager::CB_L, CBV_LGT[IND_frame].desc.BufferLocation);

                cmdlist_->SetGraphicsRootDescriptorTable(PSOManager::TEX, ResourceManager::textures_[0].tex_.HGPU);

                cmdlist_->SetGraphicsRootDescriptorTable(PSOManager::SB_MTL,SB_MTL[IND_frame].HGPU);
                cmdlist_->SetGraphicsRootDescriptorTable(PSOManager::SB_OI, SB_OI[IND_frame].HGPU);
                cmdlist_->SetGraphicsRootDescriptorTable(PSOManager::SB_MB, SB_MB[IND_frame].HGPU);

                cmdlist_->SetPipelineState(PSOManager::GetPSO(PSOManager::Shader3D::Default)->GetPSO());

                cmdlist_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                cmdlist_->RSSetViewports(1, &view_);
                cmdlist_->RSSetScissorRects(1, &rect_);

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

    cmdlist_->OMSetRenderTargets(1, &h_RTV[IND_frame], 0, nullptr);
    //cmdlist_->ClearRenderTargetView(h_RTV[IND_frame], backcolor_, 0, nullptr);

    cmdlist_->SetGraphicsRootSignature(PSOManager::GetPSO(PSOManager::ShaderPost::Default)->GetRTSG());

    cmdlist_->SetDescriptorHeaps(1, ResourceManager::DHPost_CbSrUaV->ppHeap_);
    {
        cmdlist_->SetGraphicsRootConstantBufferView(PSOManager::P_CB, CBV_Util[IND_frame].desc.BufferLocation);
    }

    cmdlist_->SetPipelineState(PSOManager::GetPSO(PSOManager::ShaderPost::Default)->GetPSO());
    cmdlist_->RSSetViewports(1, &view_);
    cmdlist_->RSSetScissorRects(1, &rect_);

    cmdlist_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    cmdlist_->IASetVertexBuffers(0, 1, &quadVBV_);

    cmdlist_->DrawInstanced(C_UI::QUAD_VERTEX, 1, 0, 0);


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

void D3d::preeffectUI()
{
    auto cnt = C_UI::GetDrawCount();
    //static auto handle = postRTV_->GetCPUDescriptorHandleForHeapStart();

    if (cnt)
    {
        {
            memcpy(SB_MB[IND_frame].view, C_UI::mb.data(), C_UI::mb.size() * sizeof(MapBOOL));
            memcpy(SB_UI[IND_frame].view, C_UI::data.data(), sizeof(SimpleInfo_UI) * C_UI::data.size());
            //SB_UI[IND_frame];
        }
//---------------------------------
        cmdlist_->OMSetRenderTargets(1,&h_RTV[IND_frame], false, nullptr);

        cmdlist_->SetGraphicsRootSignature(PSOManager::GetPSO(PSOManager::ShaderUI::Default)->GetRTSG());
        cmdlist_->SetPipelineState(PSOManager::GetPSO(PSOManager::ShaderUI::Default)->GetPSO());

        cmdlist_->SetDescriptorHeaps(1, ResourceManager::DHUI_CbSrUaV->ppHeap_);

        cmdlist_->SetGraphicsRootConstantBufferView(PSOManager::U_CB, CBV_Util[IND_frame].desc.BufferLocation);

        cmdlist_->SetGraphicsRootDescriptorTable(PSOManager::U_SB_OI, SB_UI[IND_frame].HGPU);
        cmdlist_->SetGraphicsRootDescriptorTable(PSOManager::U_SB_MB, SB_MB[IND_frame].HGPU);
        cmdlist_->SetGraphicsRootDescriptorTable(PSOManager::U_TEX, ResourceManager::textures_.data()->tex_.HGPU);

        cmdlist_->RSSetViewports(1, &view_);
        cmdlist_->RSSetScissorRects(1, &rect_);

        cmdlist_->IASetVertexBuffers(0, 1, &quadVBV_);
        cmdlist_->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        cmdlist_->DrawInstanced(C_UI::QUAD_VERTEX, cnt, 0, 0);

        cmdlist_->Close();
        ID3D12CommandList* commands[] = { cmdlist_ };
        cmdque_->ExecuteCommandLists(1, commands);

        waitGPU();
        cmdalloc_[IND_frame]->Reset();
        cmdlist_->Reset(cmdalloc_[IND_frame], nullptr);
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
   // cmdlist_->ResourceBarrier(1, &brr);
   
}

void D3d::constantUI()
{
}

namespace PTR_D3D
{
    D3d* ptr = nullptr;
}

namespace PTR_WND
{
    HWND* ptr = nullptr;
}