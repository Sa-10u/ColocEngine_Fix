#include "LightManager.h"

namespace LightManager
{
	std::vector<P_Light> PointLights;
	std::vector<D_Light> DirLights;
	std::vector<A_Light> AmbLights;
}

void LightManager::Init()
{
	PointLights = {};
	DirLights = {};
	AmbLights = {};
}

void LightManager::Term()
{
	PointLights.clear();
	DirLights.clear();
	AmbLights.clear();
}
