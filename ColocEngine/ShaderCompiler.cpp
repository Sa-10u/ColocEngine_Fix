#include "ShaderCompiler.h"
#include"FileLoader.h"
#include<vector>
#include"d3dcompiler.h"

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

bool ShaderModel6_8::Compile(std::wstring file, ID3DBlob* &blob)
{
	std::wstring str ;
	if (!FileLoad(file.c_str(), &str))	return false;

	IDxcBlobEncoding* E_blob;
	HRESULT&& res = ShaderModel6_8::util_->CreateBlob(str.c_str(), str.size(), CP_UTF8, &E_blob);
	if (FAILED(res))		return false;

	DxcBuffer buf;
	buf.Ptr = E_blob->GetBufferPointer();
	buf.Size = E_blob->GetBufferSize();
	buf.Encoding = 0;

	IDxcResult* dres;
	std::vector<LPCWSTR> args;
	args.push_back(L"-E");
	args.push_back(L"main");
	args.push_back(L"-T");
	args.push_back(L"cs_6_8");

	res = ShaderModel6_8::dxc_->Compile(&buf,args.data(), static_cast<UINT32>(args.size()),nullptr,IID_PPV_ARGS(&dres));
	if (FAILED(res))		return false;

	IDxcBlob* DXC_blob;
	res = dres->GetResult(&DXC_blob);
	if (FAILED(res))		return false;

	D3DCreateBlob(DXC_blob->GetBufferSize(), &blob);
	memcpy(blob->GetBufferPointer(), DXC_blob->GetBufferPointer(), DXC_blob->GetBufferSize());


	E_blob->Release();
	dres->Release();
	DXC_blob->Release();

	return true;
}