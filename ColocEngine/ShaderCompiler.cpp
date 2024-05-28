#include "ShaderCompiler.h"

bool ShaderModel6_8::Init()
{	
	HRESULT&& res = DxcCreateInstance(CLSID_DxcUtils,IID_PPV_ARGS(&util_));
	if (FAILED(res))		return false;

	res = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_));
	if (FAILED(res))		return false;


}

bool Compile(LPCWSTR file, IDxcBlob& blob)
{

}
