#pragma once
#include<array>
#include<typeinfo>
#include<cstdint>
#include<d3d12.h>
#include"Light.h"
#include"BUFFER.h"

static enum FLAG
{
	ON = 1,
	DISPOSAL = 2,
	USER_0 = 4,
	USER_1 = 8,
	USER_2 = 16,
	USER_3 = 32,
	USER_4 = 64,
	USER_5 = 128,
};

namespace LightManager
{
	constexpr uint16_t Lights_MAX = 256;

	struct Lights
	{
		std::array<P_Light, Lights_MAX> point;
		std::array<D_Light, Lights_MAX> dir;
		std::array<A_Light, Lights_MAX> amb;
	};
	CBUFFERVIEW<Lights> lights;

	enum KIND
	{
		P = 0,
		D,
		A,
		AMMOUNT
	};

	void Init();
	void Term();

		template<class lgt>
	uint32_t CreateLight(lgt l);

		template<class lgt>
	void DisposalLight(uint32_t ind);

	//-------------
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE HGPU;
};

namespace LightManager
{
template<class lgt>
	uint32_t CreateLight(lgt l)
	{
		auto itr = 0u;

		auto func = [&](Light* type , uint32_t size) 
		{
			if (typeid(type[0]) == typeid(l))
			{
				for (itr = 0u; itr < size; itr++) {

					if (type[itr].isDisposal()) { type[itr] = l; return true; }
				}
			}

			return false;
		};

		auto b = func(lights.ptr->point.data(), lights.ptr->point.size());

	}

template<class lgt>
	void DisposalLight(uint32_t ind)
	{
	
	};
	;
}