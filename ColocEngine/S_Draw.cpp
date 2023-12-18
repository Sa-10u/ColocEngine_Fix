#include "S_Draw.h"
#include"C_Trans.h"

void S_Draw::Draw(XMMATRIX* wld, UINT md)
{
	ResourceManager::models_[md].DrawCount_++;

	ObjInfo i;
	i.wld = *wld;

	ResourceManager::models_[md].info.push_back(i);
}

void S_Draw::Draw(C_Trans* trans, UINT md)
{
	ResourceManager::models_[md].DrawCount_++;

	ObjInfo i;
	i.wld = trans->WLDGetMTX();

	ResourceManager::models_[md].info.push_back(i);
}

void S_Draw::Draw(XMMATRIX mat, UINT md)
{
	ResourceManager::models_[md].DrawCount_++;

	ObjInfo i;
	i.wld = mat;

	ResourceManager::models_[md].info.push_back(i);
}

void S_Draw::Draw(ObjInfo* info, UINT md)
{
	ResourceManager::models_[md].DrawCount_++;
	ResourceManager::models_[md].info.push_back(*info);
}

void S_Draw::Flush(UINT md)
{
	ResourceManager::models_[md].DrawCount_ = NULL;
	ResourceManager::models_[md].info.clear();
}
