#include "ResourceManager.h"
#include"FileLoader.h"
#include"D3D.h"

namespace ResourceManager
{
	std::vector<RModel> models_;
	std::vector<RTexture> textures_;
};

void ResourceManager::Init()
{
	models_.clear();
	textures_.clear();
	
}

RModel* ResourceManager::ModelLoad(std::wstring str)
{
	for (auto list : models_) {
		if (list.Name_ == str)	return &list;
	}

	HRESULT res = E_FAIL;

	std::wstring path;
	FileLoad(str.c_str(), &path);

	RModel temp;
	
	if (!LoadMesh(path.c_str(), &temp)) return &models_[0];

    temp.VB.resize(temp.numMesh_);
    temp.IB.resize(temp.numMesh_);

	auto size = new size_t[temp.numMesh_];
	auto vtcs = new VERTEX*[temp.numMesh_];
	
	for (auto i = 0; i < temp.numMesh_; i++) {
		
		vtcs[i] = temp.Mesh_[i].vtcs_.data();
	}
    {
        D3D12_HEAP_PROPERTIES hp_prop_v = {};
        {
            hp_prop_v.Type = D3D12_HEAP_TYPE_UPLOAD;
            hp_prop_v.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            hp_prop_v.CreationNodeMask = 1;
            hp_prop_v.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            hp_prop_v.VisibleNodeMask = 1;
        };

        for (auto i = 0u; i < temp.numMesh_; i++) {
            D3D12_RESOURCE_DESC rc_desc_v = {};
            {
                *size = sizeof(VERTEX) * temp.Mesh_[i].vtcs_.size();

                rc_desc_v.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
                rc_desc_v.Alignment = 0;
                rc_desc_v.Width = *size;
                rc_desc_v.Height = 1;
                rc_desc_v.DepthOrArraySize = 1;
                rc_desc_v.MipLevels = 1;
                rc_desc_v.Format = DXGI_FORMAT_UNKNOWN;
                rc_desc_v.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
                rc_desc_v.Flags = D3D12_RESOURCE_FLAG_NONE;

                rc_desc_v.SampleDesc.Count = 1;
                rc_desc_v.SampleDesc.Quality = 0;
            }

            res = PTR_D3D::ptr->GetDevice()->CreateCommittedResource
            (
                &hp_prop_v,
                D3D12_HEAP_FLAG_NONE,
                &rc_desc_v,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&(temp.VB[i]))
            );
            if (FAILED(res))  return &models_[0];
        }
    }

}

RTexture* ResourceManager::TexLoad(std::wstring str)
{
	return nullptr;
}
