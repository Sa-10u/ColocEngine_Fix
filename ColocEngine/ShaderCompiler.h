#pragma once
#include"d3d12.h"
#include"Windows.h"
#include"dxcapi.h"

#pragma comment(lib,"dxcompiler.lib")

namespace ShaderModel6_8
{
	bool Init();

	bool Compile(LPCWSTR file,IDxcBlob& blob);

	IDxcUtils* util_;
	IDxcCompiler3* dxc_;
}