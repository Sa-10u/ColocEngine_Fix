#pragma once
#include "BUFFER.h"
#include<codecvt>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include"MACRO.h"

class MeshLoader
{
public:

	MeshLoader();
	~MeshLoader();

	bool Load(const wchar_t* file,vector<MESH>& mesh,vector<Material>& mtr ,vector<Armature>& amt);

private:

	void ParseMesh(MESH& mesh, const aiMesh* src , Mat mat , vector<Armature>& amt);
	void ParseMaterial(Material& mtl,MapBOOL& mpb ,const aiMaterial* src);
	void ParseBone(std::vector<Armature>& arm,const aiBone* src, Mat mat,MESH& mesh);
	void ParseAnim(std::vector<Armature>& arm);

	void ParseUV(aiVector3D &uv);

private:

	void UVCheck(char* str);
	void UVCheck(wchar_t* str);

	void(MeshLoader::* NormU)(ai_real& uv);
	void(MeshLoader::* NormV)(ai_real& uv);

	void Reverse(ai_real& val);
	void UnReverse(ai_real& val);
};

