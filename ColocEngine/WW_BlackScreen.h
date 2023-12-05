#pragma once
#include "WorldManager.h"

class RModel;

class WW_BlackScreen : public WorldWork
{
public:
	void Run(float tick)override;
	void Initialize()override;
	void release() override;

private:

	Entity* BG;
	RModel* md;
};

