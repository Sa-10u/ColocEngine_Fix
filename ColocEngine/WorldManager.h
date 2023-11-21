#pragma once
class Entity;
class IComp;

class WorldWork 
{
public:
	virtual void Run(float tick) =0;
	virtual void Initialize() = 0;
	virtual void release() = 0;

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
	void Init();
	void Release();

	extern WLD_ID NowWorld;

	extern WorldWork* Scene;
};

