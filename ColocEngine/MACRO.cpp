#include"MACRO.h"

#define s_path L"Resource/Shader/"

namespace SEMANTICS_STR
{
	LPCSTR POSITION = "POSITION";
	LPCSTR COLOR = "COLOR";
	LPCSTR TEXCOORD = "TEXCOORD";
	LPCSTR TANGENT = "TANGENT";
	LPCSTR NORMAL = "NORMAL";
	LPCSTR BITANGENT = "BITANGENT";
	LPCSTR MATERIAL = "MATERIAL_ID";
}

namespace SHADER_FILENAME
{
	LPCWSTR DefVS = s_path "DefVS.cso";
	LPCWSTR DefPS = s_path "DefPS.cso";
	LPCWSTR ToonPS = s_path "Toon.cso";

	LPCWSTR DefPostVS = s_path "PostEffect_Default_VS.cso";
	LPCWSTR DefPostPS = s_path "PostEffect_Default_PS.cso";

	LPCWSTR UI_VS = s_path"UI_VS.cso";
	LPCWSTR UI_PS = s_path"UI_PS.cso";
}

char* wtoc(const wchar_t* str)
{
	auto len = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
	auto buf = new char[len];

	WideCharToMultiByte(CP_UTF8, 0, str, -1, buf, len, nullptr, nullptr);

	return buf;
}

wchar_t* ctow(const char* str)
{
	auto len = MultiByteToWideChar(CP_ACP, 0, str, -1, (wchar_t*)NULL, 0);
	auto buf = new wchar_t[len];

	MultiByteToWideChar(CP_ACP, 0, str, -1, buf, len);

	return buf;
}

DH::DH(UINT incre, ID3D12DescriptorHeap** pheap) :incre_(incre), ppHeap_(pheap)
{
	h_cpu = (*ppHeap_)->GetCPUDescriptorHandleForHeapStart();
	h_gpu = (*ppHeap_)->GetGPUDescriptorHandleForHeapStart();
}

D3D12_CPU_DESCRIPTOR_HANDLE DH::GetAndIncreCPU()
{
	auto temp = h_cpu;
	h_cpu.ptr += incre_;

	return temp;
}

D3D12_GPU_DESCRIPTOR_HANDLE DH::GetAndIncreGPU()
{
	auto temp = h_gpu;
	h_gpu.ptr += incre_;

	return temp;
}

void DH::ResetHandles(D3D12_CPU_DESCRIPTOR_HANDLE h_cpu, D3D12_GPU_DESCRIPTOR_HANDLE h_gpu)
{
	this->h_cpu = h_cpu;
	this->h_gpu = h_gpu;
}

void DH::ResetIncre(UINT incre)
{
	this->incre_ = incre;
}

DH::~DH()
{
	(*ppHeap_)->Release();
}
