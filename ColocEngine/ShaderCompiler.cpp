#include "ShaderCompiler.h"
namespace ShaderModel6_8
{
	IDxcUtils * util_;
	IDxcCompiler3* dxc_;
}

bool ShaderModel6_8::Init()
{	
	HRESULT&& res = DxcCreateInstance(CLSID_DxcUtils,IID_PPV_ARGS(&util_));
	if (FAILED(res))		return false;

	res = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_));
	if (FAILED(res))		return false;

	return true;
}

bool Compile(std::string file, IDxcBlobEncoding *blob)
{
	ShaderModel6_8::util_->CreateBlob(file.c_str(), file.size(), CP_UTF8, &blob);
	return false;
}


