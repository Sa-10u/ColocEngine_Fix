#include "LightManager.h"

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

	func(lights.ptr->amb.data(), lights.ptr->amb.size());
	func(lights.ptr->dir.data(), lights.ptr->dir.size());
	func(lights.ptr->point.data(), lights.ptr->point.size());
}

void LightManager::Term()
{
	
}
