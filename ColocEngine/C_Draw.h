#pragma once
#include "IComp.h"
#include "MACRO.h"
#include"ResourceManager.h"

class C_Draw : public IComp
{
public:
	C_Draw(string tag);

	//---
	void initialize() override;
	void release() override;
	void Run(float tick) override;

private:
	RModel* mc;
	
};

