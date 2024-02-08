#pragma once
#include "WorldManager.h"
#include"BUFFER.h"

class WW_Play : public WorldWork
{
public:
	void Run(float tick)override;
	void Initialize()override;
	void release() override;


private:

};

