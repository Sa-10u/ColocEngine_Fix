#pragma once
#include "IComp.h"

class C_SetColorBG :public IComp
{
public:
	void initialize() override;
	void Run(float tick)		override;
	void release() override;

	C_SetColorBG(string tag);

};

