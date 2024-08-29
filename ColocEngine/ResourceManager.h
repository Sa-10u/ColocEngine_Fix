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

	extern RTexture E_Tex;

	extern ID3D12DescriptorHeap* heapCBV_SRV_UAV_;
	extern DH* DHH_CbSrUaV;

	extern ID3D12DescriptorHeap* heapBone_;
	extern DH* DHH_Bone;

	//-----------
	extern const uint16_t MAX_Textures;
	extern const uint16_t MAX_Models;
	extern const uint16_t MAX_AudioData;

	constexpr UINT CBCOUNT = 256;
};

