#pragma once
#include "BUFFER.h"
#include"S_Sound.h"

class RTexture
{
public:
	Texture tex_;
	std::wstring Name_;
	bool is_using;

	RTexture();
};

//------------------------------------------------

class RModel
{
public:

	uint16_t DrawCount_;
	vector<ObjInfo> info;

	std::vector<MESH> Mesh_;
	std::vector<string> TexName_;
	std::vector<Material> Mtr_;
	std::vector<ID3D12Resource*> VB;
	std::vector<ID3D12Resource*> IB;
	
	std::vector<D3D12_VERTEX_BUFFER_VIEW> VBV;
	std::vector<D3D12_INDEX_BUFFER_VIEW>  IBV;
	std::vector<Armature> armature_;

	std::wstring Name_;
	RModel();

	inline static Texture BoneMatsTex0_ {};
	inline static Texture BoneMatsTex1_ {};
	inline static Texture BoneParentTex_{};
	inline static Texture BoneLinearTex_{};
};

struct AnimData
{
	string AnimName0;
	string AnimName1;

	int32_t prog0;
	int32_t prog1;

	float linear;

	uint8_t armatureIndex;
};

class RAudioData
{
public:
	RAudioData();

	AudioData ad_;
	std::wstring Name_;

};

