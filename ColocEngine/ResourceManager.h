#pragma once
#include"Resource.h"
#include"MACRO.h"

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
	extern ID3D12DescriptorHeap* postCBV_SRV_UAV_;
	extern ID3D12DescriptorHeap* uiCBV_SRv_UAV_;

	extern DH* DHH_CbSrUaV;
	extern DH* DHPost_CbSrUaV;

	//-----------
	extern const uint16_t MAX_Textures;
	extern const uint16_t MAT_Models;

	constexpr UINT CBCOUNT = 256;

};

