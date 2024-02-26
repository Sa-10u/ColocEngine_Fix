#pragma once
#include "WorldManager.h"
#include"C_Trans.h"
#include"BUFFER.h"

class WW_BlackScreen : public WorldWork
{
public:
	void Run(float tick)override;
	void Initialize()override;
	void release() override;


private:

	void P1_BlackToWhite();
	void P2_Title();
	void P3_TitleToSelect();
	void P4_Select();

	void (WW_BlackScreen::*ptr)();


	float val0;
	float val1;

	Entity e_title;
};

