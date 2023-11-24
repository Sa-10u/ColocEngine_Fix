#pragma once
#include "BUFFER.h"

class RTexture
{
public:
	Texture tex_;
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

	std::vector<MESH> Mesh_;
	std::vector<MATERIAL> Mtr_;
	std::vector<string> TexName_;
	std::vector<bool>HasTex_;
};