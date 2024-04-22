#include "C_Player.h"
#include"S_Draw.h"
#include"ResourceManager.h"
#include<numbers>

void C_Player::initialize()
{
	md = ResourceManager::ModelLoad(L"Test.fbx");

	DataManager::CreateEntity(&this->entity, "Player");
	trans = DataManager::AddComponent<C_Trans>(&(this->entity->comps), "TRANSFORM", this->entity);
}

void C_Player::Run(float tick)
{
	trans->scale = {1,1,1};

	oi.wld = trans->WLDGetMTX();

	S_Draw::Draw(&oi, md, nullptr, NULL);
}

void C_Player::release()
{
	
}

C_Player::C_Player(string tag):IComp(tag)
{
}