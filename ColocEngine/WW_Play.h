#pragma once
#include "WorldManager.h"

class RModel;

class WW_Play : public WorldWork
{
public:
	void Run(float tick)override;
	void Initialize()override;
	void release() override;


private:

	
};

