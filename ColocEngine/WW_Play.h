#pragma once
#include "WorldManager.h"
#include"BUFFER.h"

class RModel;

class WW_Play : public WorldWork
{
public:
	void Run(float tick)override;
	void Initialize()override;
	void release() override;


private:

	int md;
	ObjInfo info;
	int md0;
};

