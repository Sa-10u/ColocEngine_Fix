#pragma once
#include "IComp.h"
#include "MACRO.h"
#include"ModelComp.h"

class C_Draw : public IComp
{
	void Draw(ModelComp* model);

	C_Draw(string tag);

	//---
	void initialize() override;
	void release() override;
	void Run(float tick) override;
};

