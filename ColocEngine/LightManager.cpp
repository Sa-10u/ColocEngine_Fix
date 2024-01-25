#include "LightManager.h"

namespace LightManager
{
	Lights lights;
}

void LightManager::Init()
{
	auto func = [&](Light* arr, uint16_t size)
	{
		const Color col = Color(.0f, .0f, .0f);
		for (auto i = 0u; i < size; i++) {
			arr[i].col = col;
			arr[i].intensity = .0f;
			arr[i].flag = DISPOSAL;
		}
	};

	func(lights.amb.data(), lights.amb.size());
	func(lights.dir.data(), lights.dir.size());
	func(lights.point.data(), lights.point.size());
}

void LightManager::Term()
{
	
}
