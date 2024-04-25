#include "C_Player.h"
#include"S_Draw.h"
#include"ResourceManager.h"
#include<numbers>
#include"C_UI.h"

void C_Player::initialize()
{
	md = ResourceManager::ModelLoad(L"Test.fbx");

	DataManager::CreateEntity(&this->entity, "Player");
	trans = DataManager::AddComponent<C_Trans>(&(this->entity->comps), "TRANSFORM", this->entity);

	auto ui = DataManager::AddComponent<C_UI>(&(this->entity->comps), "title", this->entity);

	Rect2D rect(-1, -1, 1, 1);
	ui->SetPos(rect);
}

void C_Player::Run(float tick)
{
	trans->scale = {1,1,1};
	trans->pos = {-1,0,0};

	oi.wld = trans->WLDGetMTX();

	S_Draw::Draw(&oi, md, nullptr, NULL);


}

void C_Player::release()
{
	
}

C_Player::C_Player(string tag):IComp(tag)
{
}