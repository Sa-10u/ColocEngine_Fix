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

	UINT DrawCount_;
	vector<ObjInfo> info;
	vector<BONE_INFO> currentPose_;

	std::vector<MESH> Mesh_;
	std::vector<string> TexName_;
	std::vector<ID3D12Resource*> VB;
	std::vector<ID3D12Resource*> IB;
	
	std::vector<D3D12_VERTEX_BUFFER_VIEW> VBV;
	std::vector<D3D12_INDEX_BUFFER_VIEW>  IBV;
	std::vector<Armature> armature_;

	std::wstring Name_;
	RModel();
};

class RAudioData
{
public:
	RAudioData();

	AudioData ad_;
	std::wstring Name_;

};

