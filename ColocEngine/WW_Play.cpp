#include "WW_Play.h"
#include"S_Draw.h"
#include"BUFFER.h"
#include"ResourceManager.h"

void WW_Play::Initialize()
{
	md = ResourceManager::ModelLoad(L"E_Model.fbx");
}

void WW_Play::Run(float tick)
{
	S_Draw::Draw(&info, md);
}

void WW_Play::release()
{
}
