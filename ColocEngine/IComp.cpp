#include "IComp.h"

void Entity::Update(float tick)
{
	for (auto c : comps) {

		if(c->Runnable)	c->Run(tick);
	}
}

void Entity::SubUpdate(float tick)
{
	for (auto s : sub_comps) {
		if(s->Runnable)	s->Run(tick);
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
	Tag = {};
}

//---------------------------------------------------------

void IComp::Initialize()
{
}

void IComp::Run(float tick)
{
	//do over ride!
}

void IComp::Release()
{
	//do over ride!
}

IComp::IComp(string name):entity(nullptr),Runnable(false),Usable(true)
{
	Tag = name;
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

			Entities[i].Tag = tag;

			return i;
		}
	}

	return 0;
}

Entity* DataManager::CreateEntity(size_t ID, string tag)
{
	DeleteEntity(ID);

	Entities[ID].ID = ID;
	Entities[ID].Runnable = true;
	Entities[ID].Usable = false;

	Entities[ID].Tag = tag;
	
	return &Entities[ID];
}

void DataManager::DeleteEntity(size_t ID)
{
	Entities[ID].Release();

	Entities[ID].Usable = true;
	Entities[ID].Runnable = false;
	Entities[ID].Tag = "";
}

void DataManager::DeleteEntity(Entity* e)
{
	Entities[e->ID].Release();

	Entities[e->ID].Usable = true;
	Entities[e->ID].Runnable = false;
	Entities[e->ID].Tag = "";
}

bool DataManager::IsAlive(size_t ID)
{
	return !Entities[ID].Usable;
}

bool DataManager::IsAlive(Entity* e)
{
	return !Entities[e->ID].Usable;
}

bool DataManager::IsRunnable(size_t ID)
{
	return Entities[ID].Runnable;
}

bool DataManager::IsRunnable(Entity* e)
{
	return Entities[e->ID].Runnable;
}

void DataManager::proc(float tick , size_t ID)
{
		Entities[ID].Update(tick);
}

void DataManager::sub_proc(float tick , size_t ID)
{
		Entities[ID].SubUpdate(tick);
}

void DataManager::EmptyProcess(float tick, size_t ID)
{
	//--NULL WORK--
}

void DataManager::Process(float tick)
{
	for (auto itr = 0u; itr < Size; itr++) {

		Process_Jump[Entities[itr].Runnable](tick, itr);
	}
}

void DataManager::Sub_Process(float tick)
{
	for (auto itr = 0u; itr < Size; itr++) {

		Sub_Process_Jump[Entities[itr].Runnable](tick, itr);
	}
}
