#pragma once
#include"d3d12.h"
#include"Windows.h"
#include"dxcapi.h"
#include<string>

#pragma comment(lib,"dxcompiler.lib")

namespace ShaderModel6_8
{
	bool Init();

	bool Compile(std::string file,IDxcBlobEncoding *blob);

	extern IDxcUtils* util_;
	extern IDxcCompiler3* dxc_;
}