#pragma once
#include "WorldManager.h"

class C_SetBGColor;

class WW_BlackScreen : public WorldWork
{
public:
	void Run(float tick)override;
	void Initialize()override;
	void release() override;


private:

	Entity* BG;
};

