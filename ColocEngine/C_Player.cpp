#include "C_Player.h"
#include"S_Draw.h"
#include"ResourceManager.h"
#include<numbers>

void C_Player::initialize()
{
	md = ResourceManager::ModelLoad(L"Test.fbx");
}

void C_Player::Run(float tick)
{
	oi.wld = trans.WLDGetMTX();

	S_Draw::Draw(&oi, md, nullptr, NULL);
}

void C_Player::release()
{
	
}