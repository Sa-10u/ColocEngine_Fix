#pragma once
#include"PSO.h"
#include<d3d12.h>
#include<array>

namespace PSOManager
{
	bool Init();
	void Term();

	enum class Shader3D
	{
		Default,
		Toon,

		AMMOUNT
	};

	enum class ShaderPost
	{
		Default,

		AMMOUNT
	};

	extern std::array<PSO*, static_cast<uint32_t>(Shader3D::AMMOUNT)> PSO3D;
	extern std::array<PSO*, static_cast<uint32_t>(ShaderPost::AMMOUNT)> PSOPost;

	PSO* GetPSO(Shader3D ind);
	PSO* GetPSO(ShaderPost ind);
//-------------------------------------------------

	enum
	{
		CB_U = 0,
		CB_C,
		CB_L,
		SB_MTL,
		SB_OI,
		SB_MB,
		TEX,

		Amount
	};
};
