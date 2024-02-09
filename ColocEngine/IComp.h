#pragma once
#include <string>
#include <vector>

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
};

struct IComp
{
	string Tag;

	Entity* entity;
	virtual void initialize()=0 ;
	virtual void release() = 0;
	virtual void Run(float tick);

	void Release();
	void Initialize();

	bool Runnable;
	bool Usable;

	IComp(string name);
};
//-----------------------------

namespace DataManager
{
	constexpr uint32_t Size = 200;

	extern Entity* Entities;

	size_t CreateEntity(Entity* e , string tag);
	Entity* CreateEntity(size_t ID, string tag);

	void DeleteEntity(size_t ID);
	void DeleteEntity(Entity* e);

	bool IsAlive(size_t ID);
	bool IsAlive(Entity* e);
	bool IsRunnable(size_t ID);
	bool IsRunnable(Entity* e);

	void proc(float tick , size_t ID);
	void sub_proc(float tick , size_t ID);
	void EmptyProcess(float tick , size_t ID);

	void Init();
	void Release();
	void Process(float tick);
	void Sub_Process(float tick);

	extern void (*Process_Jump[2])(float tick, size_t ID);
	extern void (*Sub_Process_Jump[2])(float tick, size_t ID);

	bool HasEntity(string tag , Entity* ptr);
	bool HasEntities(string tag, std::vector<Entity*> ptrs);

	bool HasEntity(string tag);
	uint32_t HasEntities(string tag);

	void ALL_RESET();

//----------------------------------------

	template<class t>
	t* AddComponent(t* cmp, std::vector<IComp*>* list , Entity* ent);

	template<class t>
	t* AddComponent(std::vector<IComp*>* list, string tag , Entity* ent);

	void DeleteComponent(string tag , std::vector<IComp*> *list);
	void DeleteComponent(IComp* ptr , std::vector<IComp*> *list);

	template<class t>
	void DeleteComponents(std::vector<IComp*> *list);
	void DeleteComponents(string tag , std::vector<IComp*> *list);

	bool SearchComponent(string tag , std::vector<IComp*> *list);
	bool SearchComponent(string tag, IComp* cmp , std::vector<IComp*> *list);

	uint32_t SearchComponents(string tag, std::vector<IComp*> *list);
	uint32_t SearchComponents(string tag, std::vector<IComp*> *get ,const std::vector<IComp*> *list);

	template<class t>
	uint32_t SearchComponents(std::vector<IComp*> *list);
	template<class t>
	uint32_t SearchComponents(std::vector<IComp*>* get, const std::vector<IComp*>* list);
}

//--------------------------

template<class t>
inline t* DataManager::AddComponent(t* cmp, std::vector<IComp*>* list ,Entity* ent)
{
	if (cmp == nullptr)	return nullptr;

	for (auto itr : *list) {

		if (itr->Usable)
		{
			delete itr;

			itr = cmp;
			cmp->Initialize();
			cmp->entity = ent;

			return cmp;
		}
	}

	list->push_back(cmp);
	cmp->Initialize();
	cmp->entity = ent;

	return cmp;
}
template<class t>
inline t* DataManager::AddComponent(std::vector<IComp*>* list, string tag , Entity* ent)
{
	t* cmp = new t(tag);

	for (auto itr : *list) {

		if (itr->Usable)
		{
			delete itr;

			itr = cmp;
			itr->Initialize();

			cmp->entity = ent;

			return cmp;
		}
	}

	list->push_back(cmp);
	cmp->Initialize();
	cmp->entity = ent;

	return cmp;

}

template<class t>
inline void DataManager::DeleteComponents(std::vector<IComp*>* list)
{
	for (auto itr : *list) {

		if(typeid(*itr) == typeid(t))	itr->Release();
	}
}

template<class t>
uint32_t DataManager::SearchComponents(std::vector<IComp*>* list)
{
	uint32_t cnt = 0;
	
	for (auto itr : *list) {

		if (typeid(itr) == typeid(t))	cnt++;
	}

	return cnt;
}

template<class t>
uint32_t DataManager::SearchComponents(std::vector<IComp*>* get, const std::vector<IComp*>* list)
{
	uint32_t cnt = 0;

	for (auto itr : *list) {

		if (typeid(itr) == typeid(t))
		{
			cnt++;
			get->push_back(itr);
		}
	}

	return cnt;
}