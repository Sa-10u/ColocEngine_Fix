#pragma once
#include "BUFFER.h"
#include<codecvt>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

class MeshLoader
{
public:

	MeshLoader();
	~MeshLoader();

	bool Load(const wchar_t* file,vector<MESH>& mesh,vector<MATERIAL>& mtr);
	bool Load(const wchar_t* file, RModel* ptr);

private:

	void ParseMesh(MESH& mesh, const aiMesh* src);
	void ParseMaterial(MATERIAL& mtl, const aiMaterial* src);
	void ParseBone(BONE_INFO& bns, const aiMesh* src);

	void ParseUV(aiVector3D &uv);

private:

	void UVCheck(char* str);
	void UVCheck(wchar_t* str);

	void(MeshLoader::* NormU)(ai_real& uv);
	void(MeshLoader::* NormV)(ai_real& uv);

	void Reverse(ai_real& val);
	void UnReverse(ai_real& val);
};

