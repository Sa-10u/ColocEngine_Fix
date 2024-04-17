#pragma once
#include "WorldManager.h"
#include"BUFFER.h"
#include"C_Player.h"

class WW_Play : public WorldWork
{
public:
	void Run(float tick)override;
	void Initialize()override;
	void release() override;

private:

	Entity* p_ent0;
};

