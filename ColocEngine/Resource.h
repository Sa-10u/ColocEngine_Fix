#pragma once
#include "BUFFER.h"

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

	std::vector<MESH> Mesh_;
	std::vector<Material> Mtr_;
	std::vector<string> TexName_;
	std::vector<ID3D12Resource*> VB;
	std::vector<ID3D12Resource*> IB;
	
	std::vector<D3D12_VERTEX_BUFFER_VIEW> VBV;
	std::vector<D3D12_INDEX_BUFFER_VIEW>  IBV;

	std::wstring Name_;
	RModel();
};