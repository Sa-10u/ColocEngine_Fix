#include "S_Draw.h"
#include"C_Trans.h"
#include<cassert>
#include"BUFFER.h"

//fix draw count , over instance's count then run GPU 

void S_Draw::Draw(XMMATRIX* wld, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;

	ObjInfo i;
	i.wld = *wld;

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(C_Trans* trans, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;

	ObjInfo i;
	i.wld = trans->WLDGetMTX();

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(XMMATRIX mat, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;

	ObjInfo i;
	i.wld = mat;

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(ObjInfo* info, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;
	ResourceManager::GetPointer_Mdl()[md].info.push_back(*info);
	setTex(md, mb, size);
}

void S_Draw::setTex(uint16_t md ,MapBOOL** arr, uint16_t size)
{
	auto ind = ResourceManager::GetPointer_Mdl()[md].info.size() - 1;
	auto sz = min(size, ResourceManager::GetPointer_Mdl()[md].Mesh_.size());


	for (auto i = 0u; i < sz; i++) {
		
		MapBOOL temp = {};
		temp = arr[i] == nullptr ? ResourceManager::GetPointer_Mdl()[md].Mesh_[i].defTex_ : *arr[i];

		ResourceManager::GetPointer_Mdl()[md].Mesh_[i].texIndex_.push_back(temp);
	}

	for (auto i = sz; i < ResourceManager::GetPointer_Mdl()[md].Mesh_.size(); i++) {

		ResourceManager::GetPointer_Mdl()[md].Mesh_[i].texIndex_.push_back(ResourceManager::GetPointer_Mdl()[md].Mesh_[i].defTex_);
	}
}

void S_Draw::Flush(uint16_t md)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_ = NULL;
	ResourceManager::GetPointer_Mdl()[md].info.clear();
	for (auto i = 0u; i < ResourceManager::GetPointer_Mdl()[md].Mesh_.size(); i++) {
		ResourceManager::GetPointer_Mdl()[md].Mesh_[i].texIndex_.clear();
	}
}
