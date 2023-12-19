#pragma once
#include"Light.h"
#include<vector>
#include<typeinfo>
#include<cstdint>

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
	extern std::vector<P_Light> PointLights;
	extern std::vector<D_Light> DirLights;
	extern std::vector<A_Light> AmbLights;

	void Init();
	void Term();

		template<class lgt>
	uint32_t CreateLight(lgt l);

		template<class lgt>
	void DisposalLight(uint32_t ind);
};

namespace LightManager
{
template<class lgt>
	uint32_t CreateLight(lgt l)
	{
		if (typeid(P_Light) == typeid(lgt))
		{
			for (auto ind = 0; ind < PointLights.size();ind++) {

				if (PointLights[ind].isDisposal()) { PointLights[ind] = l; PointLights[ind].flag = ON; return ind; }
			}

			PointLights.push_back(l);
			return PointLights.size() - 1;
		}

		if (typeid(D_Light) == typeid(lgt))
		{
			for (auto ind = 0; ind < DirLights.size(); ind++) {

				if (DirLights[ind].isDisposal()) { DirLights[ind] = l; DirLights[ind].flag = ON; return ind; }
			}

			DirLights.push_back(l);
			return DirLights.size() - 1;
		}

		if (typeid(A_Light) == typeid(lgt))
		{
			for (auto ind = 0; ind < AmbLights.size(); ind++) {

				if (AmbLights[ind].isDisposal()) { AmbLights[ind] = l; AmbLights[ind].flag = ON; return ind; }
			}

			AmbLights.push_back(l);
			return AmbLights.size() - 1;
		}


		return 0;
	}

template<class lgt>
	void DisposalLight(uint32_t ind)
	{
		if (typeid(P_Light) == typeid(lgt))
		{
			PointLights[ind].flag = DISPOSAL;
		}

		if (typeid(D_Light) == typeid(lgt))
		{
			DirLights[ind].flag = DISPOSAL;
		}

		if (typeid(A_Light) == typeid(lgt))
		{
			AmbLights[ind].flag = DISPOSAL;
		}
	};
	;
}