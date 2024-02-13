#pragma once
#include "C_Quad.h"
#include "ResourceManager.h"
#include<array>

struct SimpleInfo_UI
{
	Rect2D rect;

	float tick;

	float val0, val1, val2;

	SimpleInfo_UI();
};

class C_UI : public C_Quad
{
public:
	void initialize() override;
	void release() override;
	void Run(float tick) override;

	C_UI(string tag);
	C_UI(string tag , SimpleInfo_UI rect);

	bool IsInside(float x, float y);
	SimpleInfo_UI GetInfo();
	void SetInfo(SimpleInfo_UI rec);

	void Flush();

	uint16_t GetDrawCount();

public:
	static std::array<SimpleInfo_UI, ResourceManager::CBCOUNT> data;

private:
	void DrawCall();

private:
	static SIMPLEVERTEX vxcs[4];

	SimpleInfo_UI info;
	static uint16_t index;
};

