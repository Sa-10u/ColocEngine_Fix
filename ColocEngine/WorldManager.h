#pragma once
class Entity;
class IComp;

class WorldWork 
{
public:
	virtual void Run(float tick) =0;
	virtual void Initialize() = 0;
	virtual void Delete() = 0;

	void Update(float tick);
	void Release();
};

enum class WLD_ID
{
	PLAY = 0,
	SCENE,
	BLACK_SCREEN,

	//------------------------
	AMMOUNT
};

namespace WorldManager
{
	void ChangeWorld(WLD_ID next);

	WLD_ID NowWorld = WLD_ID::BLACK_SCREEN;

	WorldWork* Scene;
};

