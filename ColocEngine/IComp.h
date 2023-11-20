#pragma once
#include <string>
#include <vector>
#include <stack>

using std::string;
class IComp;

struct Entity
{
	string Tag;
	size_t ID;

	std::vector<IComp*> comps;
	std::vector<IComp*> sub_comps;

	bool Runnable;
	bool Usable;

	void Update(float tick);
	void SubUpdate(float tick);

	size_t Release();

	Entity();
	//---
	template<class t>
	t AddComponet(t cmp, std::vector<IComp*> list);
	
	template<class t>
	t AddComponent(t cmp, std::vector<IComp*> list, string tag);

};

struct IComp
{
	string Tag;

	Entity* entity;
	virtual void Initialize() ;
	virtual void Run(float tick);
	virtual void Release();

	bool Runnable;
	bool Usable;

	IComp(string name);
};
//-----------------------------

namespace DataManager
{
	constexpr uint32_t Size = 200;

	Entity Entities[Size];

	size_t CreateEntity(Entity* e , string tag);
	Entity* CreateEntity(size_t ID, string tag);

	void   DeleteEntity(size_t ID);
	void   DeleteEntity(Entity* e);

	bool IsAlive(size_t ID);
	bool IsAlive(Entity* e);
	bool IsRunnable(size_t ID);
	bool IsRunnable(Entity* e);

	void proc(float tick , size_t ID);
	void sub_proc(float tick , size_t ID);
	void EmptyProcess(float tick , size_t ID);

	void Process(float tick);
	void Sub_Process(float tick);

	void (*Process_Jump[2])(float tick ,size_t ID) = {&EmptyProcess,&proc};			//	<- use this
	void (*Sub_Process_Jump[2])(float tick,size_t ID) = {&EmptyProcess , &sub_proc};//	<- use this
}

//--------------------------

template<class t>
inline t Entity::AddComponet(t cmp, std::vector<IComp*> list)
{
	if (cmp == nullptr)	return nullptr;

	for (auto itr : list) {

		if (itr->Usable)
		{
			itr = cmp;

			return cmp;
		}
	}

	list.push_back(cmp);

	return cmp;
}
template<class t>
inline t Entity::AddComponent(t cmp, std::vector<IComp*> list, string tag)
{
	if (cmp == nullptr)
	{
		cmp = new t(tag);
		cmp->Initialize();
	}

	for (auto itr : list) {

		if (itr->Usable)
		{
			itr = cmp;

			return cmp;
		}
	}

	list.push_back(cmp);

	return cmp;

}
;
