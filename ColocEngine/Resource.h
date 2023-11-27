#pragma once
#include "BUFFER.h"

class RTexture
{
public:
	Texture tex_;

	std::wstring Name_;
};

//------------------------------------------------

class RCBuffer
{
public:
	
};

//----------------------------------

class RModel
{
public:

	UINT numMtr_;
	UINT numMesh_;
	UINT numHasTex_;
	UINT DrawCount_;

	std::vector<MESH> Mesh_;
	std::vector<MATERIAL> Mtr_;
	std::vector<string> TexName_;
	std::vector<ID3D12Resource*> VB;
	std::vector<ID3D12Resource*> IB;

	std::wstring Name_;

	
};