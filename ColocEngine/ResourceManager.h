#pragma once
#include"BUFFER.h"
#include"Resource.h"

namespace ResourceManager
{
	void Init();

	extern std::vector<RModel> models_;
	extern std::vector<RTexture> textures_;
	
	void ModelLoad(string str);
	void TexLoad(string str);
};

