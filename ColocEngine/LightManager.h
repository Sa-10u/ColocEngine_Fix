#pragma once
#include<array>
#include<typeinfo>
#include<cstdint>
#include"Light.h"

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
	extern Lights lights;

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

		if(func(lights.point.data(), lights.point.size())) return itr;
		if(func(lights.dir.data(), lights.point.size())) return itr;
		if (func(lights.amb.data(), lights.point.size())) return itr;

		return -1;
	}

template<class lgt>
	void DisposalLight(uint32_t ind)
	{
		auto func = [&](Light* arr)
		{
			if (typeid(lgt) == typeid(arr[0]))
			{
				arr[ind].flag &= ~DISPOSAL;
				arr[ind].intensity = .0f;
				arr[ind].col = Color(.0f, .0f, .0f);
				return true;
			}
			return false;
		};

		if (func(lights.point.data()))	return ;
		if (func(lights.dir.data()))	return;
		if (func(lights.amb.data()))	return;
	};
	;
}