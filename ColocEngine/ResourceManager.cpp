#include "ResourceManager.h"
#include"FileLoader.h"
#include"D3D.h"
#include<algorithm>

namespace ResourceManager
{
   
    RModel E_Model;

    std::array<RModel,MAX_Models> models_;
    std::array<RTexture,MAX_Textures> textures_;
    std::array<RAudioData, MAX_AudioData> audiodata_;
    std::array<Mat, CBCOUNT * Armature::MAX_Bones> bonemats0_;
    std::array<Mat, CBCOUNT * Armature::MAX_Bones> bonemats1_;
    std::array<uint8_t, CBCOUNT * Armature::MAX_Bones> boneparents_;
    std::array<float, CBCOUNT> bonelinears_;

    uint16_t nowLength_TEX = NULL;
    uint16_t nowLength_MDL = NULL;
    uint16_t nowLength_AD  = NULL;

    void InitBoneResource();
   
}

void ResourceManager::Init()
{
    models_ = {};
    textures_ = {};

    InitBoneResource();

    {
        Texture tex = {};

        tex.HCPU = DHH_CbSrUaV->GetAndIncreCPU();
        tex.HGPU = DHH_CbSrUaV->GetAndIncreGPU();

        MakeErrorTex(&tex);
        
        E_Tex.Name_ = L"TEXTURE_ERROR";
        E_Tex.tex_ = tex;
        E_Tex.is_using = true;

        textures_.at(0) = E_Tex;

        nowLength_TEX++;
    }
    //--------Alloc tex 

    for (auto i = 1u; i < MAX_Textures;i++) {
        Texture tex = {};

        tex.HCPU = DHH_CbSrUaV->GetAndIncreCPU();
        tex.HGPU = DHH_CbSrUaV->GetAndIncreGPU();

        RTexture temp = {};
        temp.tex_ = tex;
        temp.is_using = false;

        textures_.at(i) = temp;
    }

}

void ResourceManager::InitBoneResource()
{
    //this do init Armatures texture of bone mats
    {
        auto BoneMatsTex = [&](Texture& amtex)->void {
            D3D12_RESOURCE_DESC rsc_desc = {};
            {
                uint8_t matblock = 4u;

                rsc_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                rsc_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                rsc_desc.MipLevels = 1;
                rsc_desc.DepthOrArraySize = 1;
                rsc_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
                rsc_desc.Height = CBCOUNT;                      //256
                rsc_desc.Width = Armature::MAX_Bones * matblock;//256 * 4
                rsc_desc.SampleDesc.Count = 1;
                rsc_desc.SampleDesc.Quality = 0;
                rsc_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            }

            D3D12_HEAP_PROPERTIES hp_prop = {};
            {
                hp_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
                hp_prop.CreationNodeMask = 0;
                hp_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
                hp_prop.Type = D3D12_HEAP_TYPE_CUSTOM;
                hp_prop.VisibleNodeMask = 0;
            }

            HRESULT&& res = PTR_D3D::ptr->GetDevice()->CreateCommittedResource
            (
                &hp_prop,
                D3D12_HEAP_FLAG_NONE,
                &rsc_desc,
                D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
                nullptr,
                IID_PPV_ARGS(&amtex.rsc_ptr)
            );
            assert(!FAILED(res));

            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            {
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels = 1;
                srvDesc.Format = rsc_desc.Format;
                srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            }

            amtex.HCPU = DHH_CbSrUaV->GetAndIncreCPU();
            amtex.HGPU = DHH_CbSrUaV->GetAndIncreGPU();

            PTR_D3D::ptr->GetDevice()->CreateShaderResourceView
            (
                amtex.rsc_ptr,
                &srvDesc,
                amtex.HCPU
            );
            };

        BoneMatsTex(RModel::BoneMatsTex0_);
        BoneMatsTex(RModel::BoneMatsTex1_);
        BoneMatsTex(RModel::BoneMatsRes_);
    }

    //this do init Armatures texture of parent
    {
        Texture& patex = RModel::BoneParentTex_;

        D3D12_RESOURCE_DESC rsc_desc = {};
        {
            rsc_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            rsc_desc.Format = DXGI_FORMAT_R8_UINT;
            rsc_desc.MipLevels = 1;
            rsc_desc.DepthOrArraySize = 1;
            rsc_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
            rsc_desc.Height = CBCOUNT;                      //256
            rsc_desc.Width = Armature::MAX_Bones ;          //256
            rsc_desc.SampleDesc.Count = 1;
            rsc_desc.SampleDesc.Quality = 0;
            rsc_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        }

        D3D12_HEAP_PROPERTIES hp_prop = {};
        {
            hp_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
            hp_prop.CreationNodeMask = 0;
            hp_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
            hp_prop.Type = D3D12_HEAP_TYPE_CUSTOM;
            hp_prop.VisibleNodeMask = 0;
        }

        HRESULT&& res = PTR_D3D::ptr->GetDevice()->CreateCommittedResource
        (
            &hp_prop,
            D3D12_HEAP_FLAG_NONE,
            &rsc_desc,
            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
            nullptr,
            IID_PPV_ARGS(&patex.rsc_ptr)
        );
        assert(!FAILED(res));

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        {
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;
            srvDesc.Format = rsc_desc.Format;
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        }

        //-------------------------

        Texture& litex = RModel::BoneLinearTex_;

        {
            rsc_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
            rsc_desc.Format = DXGI_FORMAT_R16_FLOAT;
            rsc_desc.Height = 1;
            rsc_desc.Width = CBCOUNT;
        }

        res = PTR_D3D::ptr->GetDevice()->CreateCommittedResource
        (
            &hp_prop,
            D3D12_HEAP_FLAG_NONE,
            &rsc_desc,
            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
            nullptr,
            IID_PPV_ARGS(&litex.rsc_ptr)
        );
        assert(!FAILED(res));

        {
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
            srvDesc.Texture1D.MipLevels = 1;
            srvDesc.Format = rsc_desc.Format;
        }

        litex.HCPU = DHH_CbSrUaV->GetAndIncreCPU();
        litex.HGPU = DHH_CbSrUaV->GetAndIncreGPU();

        PTR_D3D::ptr->GetDevice()->CreateShaderResourceView
        (
            litex.rsc_ptr,
            &srvDesc,
            litex.HCPU
        );
    }
}

void ResourceManager::Term()
{
    //ModelFlush();
    //TexFlush();

    ALL_RELEASE_MODEL();
    ALL_RELEASE_TEX();

}

void ResourceManager::MakeErrorTex(Texture* tex)
{
    const UINT Magenta = 0xFFFF00FF;

    const auto th = 2u;     const auto tw = 2u;
    const UINT h_and_w = th * tw;

    UINT col[h_and_w];
    for (auto i = 0u; i < h_and_w; i++) {
        col[i] = Magenta;
    }

    D3D12_RESOURCE_DESC rsc_desc_tex = {};
    {
        rsc_desc_tex.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        rsc_desc_tex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        rsc_desc_tex.MipLevels = 1;
        rsc_desc_tex.DepthOrArraySize = 1;
        rsc_desc_tex.Flags = D3D12_RESOURCE_FLAG_NONE;
        rsc_desc_tex.Height = th;
        rsc_desc_tex.Width = tw;
        rsc_desc_tex.SampleDesc.Count = 1;
        rsc_desc_tex.SampleDesc.Quality = 0;
        rsc_desc_tex.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    }

    D3D12_HEAP_PROPERTIES hp_prop_tex = {};
    {
        hp_prop_tex.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
        hp_prop_tex.CreationNodeMask = 0;
        hp_prop_tex.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
        hp_prop_tex.Type = D3D12_HEAP_TYPE_CUSTOM;
        hp_prop_tex.VisibleNodeMask = 0;
    }

    HRESULT&& res = PTR_D3D::ptr->GetDevice()->CreateCommittedResource
    (
        &hp_prop_tex,
        D3D12_HEAP_FLAG_NONE,
        &rsc_desc_tex,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&tex->rsc_ptr)
    );
    assert(!FAILED(res));

    D3D12_SUBRESOURCE_DATA data = {};
    {
        data.pData = col;
        data.RowPitch = tw * sizeof(col[0]);
        data.SlicePitch = data.RowPitch * th;
    }

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = rsc_desc_tex.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    PTR_D3D::ptr->GetDevice()->CreateShaderResourceView
    (
        tex->rsc_ptr,
        &srvDesc,
        tex->HCPU
    );

    tex->rsc_ptr->WriteToSubresource
    (
        0,
        nullptr,
        data.pData,
        data.RowPitch,
        data.SlicePitch
    );
}

UINT ResourceManager::ModelLoad(std::wstring str)
{
    std::wstring path;
    if(!isResourceFile(str.c_str(), &path))    return NULL;

    auto index = -1;

    {
        bool isDecided = false;

        for (int v = 0; v < nowLength_MDL; v++) {
            if (models_[v].Name_ == path)	return v;

            if (models_[v].Name_ == L"" && !isDecided)
            {
                index = v;
                isDecided = true;
            }
        }

        if (index == -1)
        {
            nowLength_MDL = (std::min)(nowLength_MDL, MAX_Models);
            index = nowLength_MDL;
            ++nowLength_MDL;
            if (index == MAX_Models) return NULL;
        }
    }

    ID3D12Device* device_ = PTR_D3D::ptr->GetDevice();

	HRESULT&& res = E_FAIL;

	RModel temp;
	
	if (!LoadMesh(path.c_str(), &temp)) return NULL;

    temp.VB.resize(temp.Mesh_.size());
    temp.IB.resize(temp.Mesh_.size());
    temp.VBV.resize(temp.Mesh_.size());
    temp.IBV.resize(temp.Mesh_.size());
	
    for (auto i = 0; i < temp.Mesh_.size(); i++) {

        auto vtcs = temp.Mesh_[i].vtcs_.data();//
        auto size = temp.Mesh_[i].vtcs_.size() * (sizeof(VERTEX));

        {
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
                rc_desc_v.Width = size;
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
                IID_PPV_ARGS(&(temp.VB[i]))
            );
            if (FAILED(res))  return NULL;

            //----------------------------------------------------
            void* ptr = nullptr;
            res = temp.VB[i]->Map(NULL, 0, &ptr);
            if (FAILED(res)) return NULL;

            memcpy(ptr, vtcs, size);

            temp.VB[i]->Unmap(0, 0);

            temp.VBV[i].StrideInBytes = static_cast <UINT>(sizeof(VERTEX));
            temp.VBV[i].BufferLocation = temp.VB[i]->GetGPUVirtualAddress();
            temp.VBV[i].SizeInBytes = static_cast <UINT>(size);
        }
    }
        for (auto i = 0u; i < temp.Mesh_.size(); i++) {
            
            auto size = sizeof(uint32_t) * temp.Mesh_[i].indexes_.size();
            auto indcs = temp.Mesh_[i].indexes_.data();


                D3D12_HEAP_PROPERTIES hp_prop_i = {};
                {
                    hp_prop_i.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
                    hp_prop_i.CreationNodeMask = 1;
                    hp_prop_i.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
                    hp_prop_i.Type = D3D12_HEAP_TYPE_UPLOAD;
                    hp_prop_i.VisibleNodeMask = 1;
                }

                D3D12_RESOURCE_DESC rc_desc_i = {};
                {
                    rc_desc_i.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
                    rc_desc_i.Format = DXGI_FORMAT_UNKNOWN;
                    rc_desc_i.MipLevels = 1;
                    rc_desc_i.Alignment = 0;
                    rc_desc_i.DepthOrArraySize = 1;
                    rc_desc_i.Flags = D3D12_RESOURCE_FLAG_NONE;
                    rc_desc_i.Height = 1;
                    rc_desc_i.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
                    rc_desc_i.SampleDesc.Count = 1;
                    rc_desc_i.SampleDesc.Quality = 0;
                    rc_desc_i.Width = size;
                }

                res = device_->CreateCommittedResource
                (
                    &hp_prop_i,
                    D3D12_HEAP_FLAG_NONE,
                    &rc_desc_i,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(&temp.IB[i])
                );
                if (FAILED(res)) return NULL;

                void* ptr = nullptr;
                res = temp.IB[i]->Map(0, nullptr, &ptr);
                if (FAILED(res)) return NULL;

                memcpy(ptr, indcs, size);
                temp.IB[i]->Unmap(0, 0);

                //-----------
                temp.IBV[i].BufferLocation = temp.IB[i]->GetGPUVirtualAddress();
                temp.IBV[i].Format = DXGI_FORMAT_R32_UINT;
                temp.IBV[i].SizeInBytes = size;
         }

            temp.Name_ = path.c_str();


        ResourceManager::models_.at(index) = temp;
        return index;
    
}

UINT ResourceManager::TexLoad(std::wstring str)
{
    std::wstring path;
    if (!isResourceFile(str.c_str(), &path))    return NULL;

    ID3D12Device* device_ = PTR_D3D::ptr->GetDevice();

    auto index = -1;
    
    {
        bool isDecided = false;

        for (auto v = 0u; v < nowLength_TEX; v++) {
            if (textures_[v].Name_ == path)	return v;


            if (!textures_[v].is_using && !isDecided)
            {
                index = v;
                isDecided = true;
            }
        }

        if (index == -1)
        {
            nowLength_TEX = (std::min)(nowLength_TEX, MAX_Models);
            index = nowLength_TEX;
            ++nowLength_TEX;
            if (index == MAX_Textures) return NULL;
        }
    }
    HRESULT&& res = E_FAIL;

    res = E_FAIL;
    //-------
    TexMetadata data = {};
    ScratchImage scr = {};
    const Image* image = {}
    ;
    res = LoadFromWICFile
    (
        path.c_str(),
        WIC_FLAGS_NONE,
        &data,
        scr
    );
    if (FAILED(res)) return NULL;

    image = scr.GetImage(0, 0, 0);
   // scr.Release();

    D3D12_RESOURCE_DESC rc_desc_tex = {};
    {
        rc_desc_tex.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        rc_desc_tex.Format = image->format;
        rc_desc_tex.MipLevels = 1;
        rc_desc_tex.DepthOrArraySize = 1;
        rc_desc_tex.Flags = D3D12_RESOURCE_FLAG_NONE;
        rc_desc_tex.Height = image->height;
        rc_desc_tex.Width = image->width;
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
        IID_PPV_ARGS(&textures_.at(index).tex_.rsc_ptr)
    );
    if (FAILED(res)) return NULL;

    textures_.at(index).tex_.rsc_ptr->WriteToSubresource
    (
        0,nullptr,
        image->pixels,
        image->rowPitch,
        image->slicePitch
    );

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

    textures_.at(index).Name_ = path.c_str();

    device_->CreateShaderResourceView
    (
        textures_.at(index).tex_.rsc_ptr,
        &rsc_v_desc,
        textures_.at(index).tex_.HCPU
    );
    textures_.at(index).is_using = true;
    scr.Release();

    return index;
}

UINT ResourceManager::ADLoad(std::wstring str)
{
    auto index_ = 0u;

    for (auto i = 0u;i<nowLength_AD;i++) {

        if (audiodata_[i].Name_ == str)     return i;

        if (audiodata_[i].Name_ == L"")     { index_ = i; break; }
    }

    return S_Sound::LoadWave(str,&audiodata_[index_].ad_) ? index_ : NULL;

}

void ResourceManager::ModelFlush()
{
    for (auto& md : models_) {

        md.Name_.clear();
        md.VB.clear();
        md.IB.clear();
        md.TexName_.clear();
        md.Mesh_.clear();
    }
}

void ResourceManager::TexFlush()
{
    for (auto& itr : textures_) {

        if (itr.tex_.rsc_ptr != NULL && itr.tex_.rsc_ptr != E_Tex.tex_.rsc_ptr)
        {
            itr.tex_.rsc_ptr->Release();
            itr.Name_.clear();
            itr.is_using = false;
        }

        itr = {};
    }

    textures_.at(0) = E_Tex;

}

void ResourceManager::ADFlush()
{
    for (auto& itr : audiodata_) {
        itr = {};
    }
}

void ResourceManager::ALL_RELEASE_MODEL()
{
    for (auto& itr : models_) {
        for (auto i = 0u; i < itr.Mesh_.size(); i++) {

            itr.Mesh_[i].vtcs_.clear();
            itr.Mesh_[i].indexes_.clear();

            itr.IB[i]->Release();
            itr.VB[i]->Release();
        }
        itr.IB.clear();
        itr.VB.clear();

        itr.VBV.clear();
        itr.IBV.clear();
        itr.info.clear();
        itr.Mesh_.clear();
        itr.Name_.clear();

        itr = {};
    }
}

void ResourceManager::ALL_RELEASE_TEX()
{
    for (auto& itr : textures_) {

        if (itr.tex_.rsc_ptr != NULL)
        {
            itr.tex_.rsc_ptr->Release();
            itr.Name_.clear();
        }

        itr = {};
    }
}

void ResourceManager::ALL_RELEASE_AD()
{
    for (auto& itr : audiodata_) {
        itr = {};
    }
}

RTexture* ResourceManager::GetPointer_Tex()
{
    return textures_.data();
}

RModel* ResourceManager::GetPointer_Mdl()
{
    return models_.data();
}

RAudioData* ResourceManager::GetPointer_Ad()
{
    return audiodata_.data();
}
