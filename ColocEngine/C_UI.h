#pragma once
#include "C_Quad.h"

class C_UI : public C_Quad
{
public:
	void initialize() override;
	void release() override;
	void Run(float tick) override;

	C_UI(string tag);
	C_UI(string tag , Rect2D rect);

public:


private:
	SIMPLEVERTEX vxcs[4];
};

