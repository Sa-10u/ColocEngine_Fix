#pragma once
#include "I_move.h"
class C_P_move : public I_move
{
	void Jump() override;
	void Walk() override;
	void Dash() override;
}

