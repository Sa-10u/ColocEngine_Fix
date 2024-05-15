#pragma once
#include"MACRO.h"
#include"DirectXMath.h"
#include<d3d12.h>
#include <string>
#include <vector>
#include<cstdint>
#include"Resource.h"

namespace ResourceManager
{
	void Init();
	void Term();

	void MakeErrorTex(Texture* tex);


	extern std::vector<RModel> models_;
	extern std::vector<RTexture> textures_;	//fix vector to array
	
	UINT ModelLoad(std::wstring str);
	UINT TexLoad(std::wstring str);

	void ModelFlush();
	void TexFlush();
	
	void ALL_RELEASE_MODEL();
	void ALL_RELEASE_TEX();

	extern RTexture E_Tex;

	extern ID3D12DescriptorHeap* heapCBV_SRV_UAV_;
	extern DH* DHH_CbSrUaV;

	//-----------
	extern const uint16_t MAX_Textures;
	extern const uint16_t MAT_Models;

	constexpr UINT CBCOUNT = 256;

};

