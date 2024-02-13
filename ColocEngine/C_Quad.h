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

public:

	uint32_t* GetIndcsData();


protected:

	static Position vxs[4];
	static uint32_t ids[6];

};

