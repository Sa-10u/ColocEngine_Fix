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
	struct alignas(256) Lights
	{
		P_Light point;
		D_Light dir;
		A_Light amb;
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
	uint32_t CreateLight();

		template<class lgt>
	void DisposalLight(uint32_t ind);

	//-------------
};

namespace LightManager
{
template<class lgt>
	uint32_t CreateLight()
	{
		auto itr = 0u;

		auto func = [&](Light* type ) 
		{
			if (typeid(*type) == typeid(lgt))
			{
				for (itr = 0u; itr < Lights_MAX; itr++) {

					if (type->isDisposal(itr))
					{
						type->Reset(itr);
						return true;
					}
				}
			}

			return false;
		};

		if(func(&lights.point)) return itr;
		if(func(&lights.dir)) return itr;
		if(func(&lights.amb)) return itr;

		return -1;
	}

template<class lgt>
	void DisposalLight(uint32_t ind)
	{
		auto func = [&](Light* type)
		{
			if (typeid(lgt) == typeid(*type))
			{
				type->ToON(ind);
				type->ToStore(ind);
				return true;
			}
			return false;
		};

		if (func(&lights.point))	return ;
		if (func(&lights.dir))	return;
		if (func(&lights.amb))	return;
	};
}