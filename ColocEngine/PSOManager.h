#pragma once
#include"PSO.h"
#include<d3d12.h>
#include<array>

namespace PSOManager
{
	bool Init();
	void Term();

	enum class ShaderDeferred
	{
		Default ,

		AMOUNT
	};

	enum class Shader3D
	{
		Default,

		AMOUNT
	};

	enum class ShaderPost
	{
		Default,

		AMOUNT
	};

	enum class ShaderUI
	{
		Default,

		AMOUNT
	};

	extern std::array<PSO*, static_cast<uint32_t>(ShaderDeferred::AMOUNT)> PSODeferred;
	extern std::array<PSO*, static_cast<uint32_t>(Shader3D::AMOUNT)> PSO3D;
	extern std::array<PSO*, static_cast<uint32_t>(ShaderPost::AMOUNT)> PSOPost;
	extern std::array<PSO*, static_cast<uint32_t>(ShaderUI::AMOUNT)>PSOUI;

	PSO* GetPSO(ShaderDeferred ind);
	PSO* GetPSO(Shader3D ind);
	PSO* GetPSO(ShaderPost ind);
	PSO* GetPSO(ShaderUI ind);

//-------------------------------------------------

	enum
	{
		D_CB_U = 0,
		D_CB_C,
		D_CB_L,
		D_TEX,
		D_RENDER,

		D_Amount
	};

	enum 
	{
		CB_U = 0,
		CB_C,
		SB_MTL,
		SB_OI,
		SB_MB,
		TEX,

		Amount
	};

	enum
	{
		P_CB = 0,
		P_RENDER,
		P_TEX,
		P_SB_MB,

		P_Amount
	};

	enum
	{
		U_CB = 0,
		U_SB_OI ,
		U_SB_MB ,
		U_TEX,

		U_Amount
	};
};
