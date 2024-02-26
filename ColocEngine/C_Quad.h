#pragma once
#include "IComp.h"
#include"BUFFER.h"
#include"C_Trans.h"

class C_Quad : public IComp
{
public:

	void initialize() override;
	void release() override;
	void Run(float tick) override;

	C_Quad(string tag);

	static constexpr uint8_t QUAD_VERTEX = 4;
	static constexpr uint8_t QUAD_INDEX = 6;

public:

	uint32_t* GetIndcsData();


protected:

	static Position vxs[C_Quad::QUAD_VERTEX];
	static uint32_t ids[C_Quad::QUAD_INDEX];

};

