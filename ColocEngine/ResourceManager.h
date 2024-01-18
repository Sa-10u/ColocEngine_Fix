#pragma once
#include"Resource.h"
#include"MACRO.h"

namespace ResourceManager
{
	void Init();
	void Term();

	void MakeErrorTex(Texture* tex);


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

	extern DH* heap_;

	//-----------
	extern const uint16_t MAX_Textures;
	extern const uint16_t MAT_Models;
};

