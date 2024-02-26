#pragma once
#include "C_Quad.h"
#include "ResourceManager.h"
#include<array>

struct alignas(16) SimpleInfo_UI
{
	Rect2D rect;
	Rect2D cutUV;

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
	void SetTexture(MapBOOL mb);

	void SetPos(Rect2D rect);
	void SetUV(Rect2D rect);
	void SetTick(float tick);
	void SetValue(float v0, float v1, float v2);

	static void Flush();

	static uint16_t GetDrawCount();
	static SIMPLEVERTEX vxcs[C_Quad::QUAD_VERTEX];


public:
	static std::array<SimpleInfo_UI, ResourceManager::CBCOUNT> data;
	static std::array<MapBOOL, ResourceManager::CBCOUNT> mb;

private:
	inline void DrawCall();

private:

	MapBOOL pic;
	SimpleInfo_UI info;
	static uint16_t index;
};

