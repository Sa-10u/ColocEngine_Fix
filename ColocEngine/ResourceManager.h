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

	UINT ModelLoad(std::wstring str);
	UINT TexLoad(std::wstring str);
	UINT ADLoad(std::wstring str);

	void ModelFlush();
	void TexFlush();
	void ADFlush();
	
	void ALL_RELEASE_MODEL();
	void ALL_RELEASE_TEX();
	void ALL_RELEASE_AD();

	RTexture* GetPointer_Tex();
	RModel* GetPointer_Mdl();
	RAudioData* GetPointer_Ad();

	inline RTexture E_Tex{};

	inline ID3D12DescriptorHeap* heapCBV_SRV_UAV_{};
	inline DH* DHH_CbSrUaV{};

	//-----------
	inline const uint16_t MAX_Textures = 1024;
	inline const uint16_t MAX_Models = 1024;
	inline const uint16_t MAX_AudioData = 1024;

	constexpr UINT CBCOUNT = 256;
};

