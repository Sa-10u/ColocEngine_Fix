#include "ShaderCompiler.h"
#include"FileLoader.h"
#include<vector>
#include"d3dcompiler.h"
#include<fstream>
#include"MACRO.h"

#if _DebugCUI
#include<iostream>
#endif

namespace ShaderModel6_8
{
	IDxcUtils* util_;
	IDxcCompiler3* dxc_;
	IDxcIncludeHandler* inc_;
}

bool ShaderModel6_8::Init()
{
	HRESULT&& res = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&util_));
	if (FAILED(res))		return false;

	res = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_));
	if (FAILED(res))		return false;

	res = util_->CreateDefaultIncludeHandler(&inc_);
	if (FAILED(res))		return false;

	return true;
}

bool ShaderModel6_8::Compile(std::wstring file, ID3DBlob*& blob, ShaderCompileType sct)
{
	std::wstring str;
	if (!isResourceFile(file.c_str(), &str))	return false;

	std::ifstream filecode(str, std::ios::binary);
	std::string code = std::string(std::istreambuf_iterator<char>(filecode), std::istreambuf_iterator<char>());

	IDxcBlobEncoding* E_blob;
	HRESULT&& res = ShaderModel6_8::util_->CreateBlob(code.c_str(), code.size(), CP_UTF8, &E_blob);
	if (FAILED(res))		return false;

	DxcBuffer buf;
	buf.Ptr = E_blob->GetBufferPointer();
	buf.Size = E_blob->GetBufferSize();
	buf.Encoding = DXC_CP_ACP;

	IDxcResult* dres;
	std::vector<LPCWSTR> args;
	args.push_back(L"-E");
	args.push_back(L"main");
	args.push_back(L"-T");
	switch (sct)
	{
	case ShaderCompileType::Vertex: args.push_back(L"vs_6_8"); break;

	case ShaderCompileType::Pixel: args.push_back(L"ps_6_8"); break;

	case ShaderCompileType::Compute: args.push_back(L"cs_6_8"); break;
	}
	//args.push_back(L"-enable-16bit-types");

	res = ShaderModel6_8::dxc_->Compile(&buf, args.data(), static_cast<UINT32>(args.size()), inc_, IID_PPV_ARGS(&dres));
	if (FAILED(res))		return false;

	IDxcBlob* DXC_blob;
	res = dres->GetResult(&DXC_blob);
	if (FAILED(res))		return false;

#if _DebugCUI
	IDxcBlobUtf8* error;
	dres->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&error), nullptr);
	auto e = error->GetStringPointer();
	std::cout << e;
#endif

	auto s = DXC_blob->GetBufferSize();
	D3DCreateBlob(DXC_blob->GetBufferSize(), &blob);
	memcpy(blob->GetBufferPointer(), DXC_blob->GetBufferPointer(), DXC_blob->GetBufferSize());

	E_blob->Release();
	dres->Release();
	DXC_blob->Release();

	return true;
}