#pragma once
#include "C_Quad.h"
#include "ResourceManager.h"
#include<array>

class C_Billboard : public C_Quad
{
public:
	void initialize() override;
	void release() override;
	void Run(float tick) override;

	C_Billboard(string tag);
	C_Billboard(string tag, SimpleQuad rect);

	SimpleQuad GetInfo();
	void SetInfo(SimpleQuad rec);
	void SetTexture(MapBOOL mb);

	void SetPos(Rect2D rect);
	void SetUV(Rect2D rect);
	void SetTick(float tick);
	void SetValue(float v0, float v1, float v2);

	static void Flush();

	static uint16_t GetDrawCount();
	static SIMPLEVERTEX vxcs[C_Quad::QUAD_VERTEX];

public:
	static std::array<SimpleQuad, ResourceManager::CBCOUNT> data;
	static std::array<MapBOOL, ResourceManager::CBCOUNT> mb;

private:
	inline void DrawCall();

private:

	MapBOOL pic;
	SimpleQuad info;
	static uint16_t index;
};

