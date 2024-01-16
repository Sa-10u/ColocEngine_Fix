#include "ResourceManager.h"
#include"FileLoader.h"
#include"D3D.h"

namespace ResourceManager
{
    std::vector<RModel> models_;
    std::vector<RTexture> textures_;
    RModel E_Model;
    RTexture E_Tex;
}

void ResourceManager::Init()
{
	models_.clear();
	textures_.clear();

    {
        Texture tex = {};
        const UINT Magenta = 0xFFFF00FF;

        const auto th = 2u;     const auto tw = 2u;
        const UINT h_and_w = th * tw;

        UINT col[h_and_w];
        for (auto i = 0u; i < h_and_w; i++) {
            col[i] = Magenta;
        }

        D3D12_RESOURCE_DESC rc_desc_tex = {};
        {
            rc_desc_tex.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            rc_desc_tex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            rc_desc_tex.MipLevels = 1;
            rc_desc_tex.DepthOrArraySize = 1;
            rc_desc_tex.Flags = D3D12_RESOURCE_FLAG_NONE;
            rc_desc_tex.Height = th;
            rc_desc_tex.Width = tw;
            rc_desc_tex.SampleDesc.Count = 1;
            rc_desc_tex.SampleDesc.Quality = 0;
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

        HRESULT res = PTR_D3D::ptr->GetDevice()->CreateCommittedResource
        (
            &hp_prop_tex,
            D3D12_HEAP_FLAG_NONE,
            &rc_desc_tex,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&tex.rsc_ptr)
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
        srvDesc.Format = rc_desc_tex.Format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        PTR_D3D::ptr->GetDevice()->CreateShaderResourceView
            (
                tex.rsc_ptr,
                &srvDesc,
                tex.HCPU
            );
    
    }
}

void ResourceManager::Term()
{
    ModelFlush();
    TexFlush();

    ALL_RELEASE_MODEL();
    ALL_RELEASE_TEX();
}

UINT ResourceManager::ModelLoad(std::wstring str)
{
    std::wstring path;
    if(!FileLoad(str.c_str(), &path))    return 0;

    for (int v = 0; v < models_.size();v++) {
		if (models_[v].Name_ == path)	return v;
	}

    ID3D12Device* device_ = PTR_D3D::ptr->GetDevice();

	HRESULT res = E_FAIL;

	RModel temp;
	
	if (!LoadMesh(path.c_str(), &temp)) return 0;

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
            if (FAILED(res))  return 0;

            //----------------------------------------------------
            void* ptr = nullptr;
            res = temp.VB[i]->Map(NULL, 0, &ptr);
            if (FAILED(res)) return 0;

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
                if (FAILED(res)) return 0;

                void* ptr = nullptr;
                res = temp.IB[i]->Map(0, nullptr, &ptr);
                if (FAILED(res)) return 0;

                memcpy(ptr, indcs, size);
                temp.IB[i]->Unmap(0, 0);

                //-----------
                temp.IBV[i].BufferLocation = temp.IB[i]->GetGPUVirtualAddress();
                temp.IBV[i].Format = DXGI_FORMAT_R32_UINT;
                temp.IBV[i].SizeInBytes = size;
         }

            temp.Name_ = path.c_str();


        ResourceManager::models_.push_back(temp);
        return ResourceManager::models_.size() - 1;
    
}

UINT ResourceManager::TexLoad(std::wstring str)
{
    ID3D12Device* device_ = PTR_D3D::ptr->GetDevice();

    for (auto v = 0u; v < textures_.size();v++) {
        if (textures_[v].Name_ == str)	return v;
    }

    RTexture temp = {};
    HRESULT res = E_FAIL;

    std::wstring path;
    FileLoad(str.c_str(), &path);

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
    if (FAILED(res)) return 0;

    image = scr.GetImage(0, 0, 0);

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
        IID_PPV_ARGS(&temp.tex_.rsc_ptr)
    );
    if (FAILED(res)) return 0;

}

void ResourceManager::ModelFlush()
{
    for (auto md : models_) {

        md.Name_.clear();
        md.VB.clear();
        md.IB.clear();
        md.TexName_.clear();
        md.Mtr_.clear();
        md.Mesh_.clear();
    }
}

void ResourceManager::TexFlush()
{
    for (auto pic : textures_) {

        pic.Name_.clear();
        pic.DHeap->Release();
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
        itr.Mtr_.clear();
       
    }

    models_.clear();
}

void ResourceManager::ALL_RELEASE_TEX()
{
    for (auto& itr : textures_) {
        itr.tex_.rsc_ptr->Release();
        itr.DHeap->Release();
    }

    textures_.clear();
}
