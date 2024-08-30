#pragma once
#include"d3d12.h"
#include"Windows.h"
#include"dxcapi.h"
#include<string>

#pragma comment(lib,"dxcompiler.lib")

enum class ShaderCompileType
{
	Vertex,
	Pixel,
	Compute,

};

namespace ShaderModel6_8
{
	bool Init();

	bool Compile(std::wstring file, ID3DBlob*& blob, ShaderCompileType sct);

	extern IDxcUtils* util_;
	extern IDxcCompiler3* dxc_;
	extern IDxcIncludeHandler* inc_;
}