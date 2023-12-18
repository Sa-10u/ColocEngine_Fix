#pragma once
#include"BUFFER.h"
#include"Resource.h"

namespace ResourceManager
{
	void Init();
	void Term();

	extern std::vector<RModel> models_;
	extern std::vector<RTexture> textures_;
	
	UINT ModelLoad(std::wstring str);
	UINT TexLoad(std::wstring str);

	void ModelFlush();
	void TexFlush();
	
	void ALL_RELEASE_MODEL();
	void ALL_RELEASE_TEX();

	extern RModel E_Model;
	extern RTexture E_Tex;
};

