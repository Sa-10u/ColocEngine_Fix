#include "S_Draw.h"
#include"C_Trans.h"

void S_Draw::Draw(XMMATRIX* wld, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::models_[md].DrawCount_++;

	ObjInfo i;
	i.wld = *wld;

	ResourceManager::models_[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(C_Trans* trans, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::models_[md].DrawCount_++;

	ObjInfo i;
	i.wld = trans->WLDGetMTX();

	ResourceManager::models_[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(XMMATRIX mat, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::models_[md].DrawCount_++;

	ObjInfo i;
	i.wld = mat;

	ResourceManager::models_[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(ObjInfo* info, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::models_[md].DrawCount_++;
	ResourceManager::models_[md].info.push_back(*info);
	setTex(md, mb, size);
}

void S_Draw::setTex(uint16_t md ,MapBOOL** arr, uint16_t size)
{
	auto ind = ResourceManager::models_[md].info.size() - 1;
	auto sz = min(size, ResourceManager::models_[md].Mesh_.size());


	for (auto i = 0u; i < sz; i++) {
		
		MapBOOL temp = {};
		temp = arr[i] == nullptr ? ResourceManager::models_[md].Mesh_[i].defTex_ : *arr[i];

		ResourceManager::models_[md].Mesh_[i].texIndex_.push_back(temp);
	}

	for (auto i = sz; i < ResourceManager::models_[md].Mesh_.size(); i++) {

		ResourceManager::models_[md].Mesh_[i].texIndex_.push_back(ResourceManager::models_[md].Mesh_[i].defTex_);
	}
}

void S_Draw::Flush(uint16_t md)
{
	ResourceManager::models_[md].DrawCount_ = NULL;
	ResourceManager::models_[md].info.clear();
	for (auto i = 0u; i < ResourceManager::models_[md].Mesh_.size(); i++) {
		ResourceManager::models_[md].Mesh_[i].texIndex_.clear();
	}
}
