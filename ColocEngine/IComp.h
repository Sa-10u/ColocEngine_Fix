#pragma once
#include <string>
#include <vector>
#include <stack>

using std::string;
class IComp;

struct Entity
{
	string tag;
	size_t ID;

	std::vector<IComp*> comps;
	std::vector<IComp*> sub_comps;

	void Update(uint32_t tick);
	void SubUpdate(uint32_t tick);

	size_t Release();

	Entity();

	bool Runnable;
	bool Usable;
};

struct IComp
{
	Entity* entity;
	virtual void Run(uint32_t tick);
	virtual void Release();

	bool Runnable;
	bool Usable;

	IComp();
};
//-----------------------------

namespace DataManager
{
	constexpr uint32_t Size = 200;

	Entity Entities[Size];

	size_t CreateEntity(Entity* e , string tag);
	void   DeleteEntity(size_t ID);
	void   DeleteEntity(Entity* e);

	bool IsAlive(size_t ID);
	bool IsRunnable(size_t ID);

	void Process();
	void Sub_Process();
}