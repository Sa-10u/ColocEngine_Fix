#pragma once
#include"BUFFER.h"
#include"Resource.h"

namespace ResourceManager
{
	void Init();
	void Term();

	extern std::vector<RModel> models_;
	extern std::vector<RTexture> textures_;
	
	RModel* ModelLoad(std::wstring str);
	RTexture* TexLoad(std::wstring str);

	void ModelFlush();
	void TexFlush();
};

