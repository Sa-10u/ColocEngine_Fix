#pragma once
#include"IComp.h"

class C_Text : public IComp
{
public:

	C_Text(string tag);

	void initialize() override;
	void release() override;
	void Run(float tick) override;

private:
};

