#include "IComp.h"

void Entity::Update(uint32_t tick)
{
	for (auto c : comps) {

		c->Run(tick);
	}
}

void Entity::SubUpdate(uint32_t tick)
{
	for (auto s : sub_comps) {
		s->Run(tick);
	}
}

size_t Entity::Release()
{
	this->Usable = true;
	this->Runnable = false;

	for (auto e : comps) {
		e->Release();
	}

	for (auto s : sub_comps) {
		s->Release();
	}

	return ID;
}

Entity::Entity():Usable(true),Runnable(false),ID(NULL)
{
	tag = {};
}

//---------------------------------------------------------

void IComp::Run(uint32_t tick)
{
	//do over ride!
}

void IComp::Release()
{
	//do over ride!
}

IComp::IComp():entity(nullptr),Runnable(false),Usable(true)
{
}

//-------------------------------

size_t DataManager::CreateEntity(Entity* e , string tag)
{
	for (auto i = 0; i < Size; i++) {

		if (Entities[i].Usable)
		{
			Entities[i].ID = i;
			Entities[i].Runnable = true;
			Entities[i].Usable = false;

			Entities[i].tag = tag;

			return i;
		}
	}
}

void DataManager::DeleteEntity(size_t ID)
{
}

void DataManager::DeleteEntity(Entity* e)
{
}

bool DataManager::IsAlive(size_t ID)
{
	return false;
}

bool DataManager::IsRunnable(size_t ID)
{
	return false;
}

void DataManager::Process()
{
}

void DataManager::Sub_Process()
{
}
