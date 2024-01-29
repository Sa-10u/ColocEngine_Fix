#pragma once
#include"PSO.h"
#include<d3d12.h>
#include<array>

namespace PSOManager
{
	void Init();

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

	extern std::array<ID3D12PipelineState*, static_cast<uint32_t>(Shader3D::AMMOUNT)> PSO3D;
	extern std::array<ID3D12PipelineState*, static_cast<uint32_t>(ShaderPost::AMMOUNT)> PSOPost;

	ID3D12PipelineState* GetPSO(Shader3D ind);
	ID3D12PipelineState* GetPSO(ShaderPost ind);
};
