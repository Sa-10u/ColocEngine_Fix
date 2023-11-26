#pragma once
#include "BUFFER.h"

class RTexture
{
public:
	Texture tex_;

	string Name_;
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

	string Name_;

	
};