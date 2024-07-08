#include"MACRO.h"
#include<cmath>
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
	LPCWSTR DefDeferredVS = L"Deferred_Def_VS.hlsl";
	LPCWSTR DefDeferredPS = L"Deferred_Def_PS.hlsl";

	LPCWSTR DefVS = L"DefVS.hlsl";
	LPCWSTR DefPS = L"DefPS.hlsl";

	LPCWSTR DefPostVS = L"PostEffect_Default_VS.hlsl";
	LPCWSTR DefPostPS = L"PostEffect_Default_PS.hlsl";

	LPCWSTR UI_VS = L"UI_VS.hlsl";
	LPCWSTR UI_PS = L"UI_PS.hlsl";
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

//---------------------------------------------------

using std::fmod;
float3 operator+(const float3& f0, const float3& f1)
{
	return float3(f0.x + f1.x, f0.y + f1.y, f0.z + f1.z);
}
float3 operator-(const float3& f0, const float3& f1)
{
	return float3(f0.x - f1.x, f0.y - f1.y, f0.z - f1.z);
}
float3 operator*(const float3& f0, const float3& f1)
{
	return float3(f0.x * f1.x, f0.y * f1.y, f0.z * f1.z);
}
float3 operator/(const float3& f0, const float3& f1)
{
	return float3(f0.x / f1.x, f0.y / f1.y, f0.z / f1.z);
}

float3 operator%(const float3& f0, const float3& f1)
{
	return float3(fmod(f0.x, f1.x), fmod(f0.y, f1.y), fmod(f0.z, f1.z));
}

bool operator==(const float3& f0, const float3& f1)
{
	return f0.x == f1.x && f0.y == f1.y && f0.z == f1.z;
}

bool operator!=(const float3& f0, const float3& f1)
{
	return f0.x != f1.x && f0.y != f1.y && f0.z != f1.z;
}

float3 operator++(float3& f0)
{
	f0 = float3(f0.x + 1.0f, f0.y + 1.0f, f0.z + 1.0f);
	return f0;
}

float3 operator++(float3& f0, int dummy)
{
	float3 temp = f0;
	f0 = float3(f0.x + 1.0f, f0.y + 1.0f, f0.z + 1.0f);
	return temp;
}

float3 operator--(float3& f0)
{
	f0 = float3(f0.x - 1.0f, f0.y - 1.0f, f0.z - 1.0f);
	return f0;
}

float3 operator--(float3& f0, int dummy)
{
	float3 temp = f0;
	f0 = float3(f0.x - 1.0f, f0.y - 1.0f, f0.z - 1.0f);
	return temp;
}

void operator+=(float3& f0, const float3& f1)
{
	f0 = float3(f0.x + f1.x, f0.y + f1.y, f0.z + f1.z);
}

void operator-=(float3& f0, const float3& f1)
{
	f0 = float3(f0.x - f1.x, f0.y - f1.y, f0.z - f1.z);
}

void operator*=(float3& f0, const float3& f1)
{
	f0 = float3(f0.x * f1.x, f0.y * f1.y, f0.z * f1.z);
}

void operator/=(float3& f0, const float3& f1)
{
	f0 = float3(f0.x / f1.x, f0.y / f1.y, f0.z / f1.z);
}

void operator%=(float3& f0, const float3& f1)
{
	f0 = float3(fmod(f0.x, f1.x), fmod(f0.y, f1.y), fmod(f0.z, f1.z));
}

float3 operator+(const float3& f0, const float& f1)
{
	return float3(f0.x + f1, f0.y + f1, f0.z + f1);
}

float3 operator-(const float3& f0, const float& f1)
{
	return float3(f0.x - f1, f0.y - f1, f0.z - f1);
}

float3 operator*(const float3& f0, const float& f1)
{
	return float3(f0.x * f1, f0.y * f1, f0.z * f1);
}

float3 operator/(const float3& f0, const float& f1)
{
	return float3(f0.x / f1, f0.y / f1, f0.z / f1);
}

float3 operator%(const float3& f0, const float& f1)
{
	return float3(fmod(f0.x,f1), fmod(f0.y,f1), fmod(f0.z,f1));
}

float3 operator==(const float3& f0, const float& f1)
{
	return float3(f0.x == f1, f0.y == f1, f0.z == f1);
}

float3 operator!=(const float3& f0, const float& f1)
{
	return float3(f0.x != f1, f0.y != f1, f0.z != f1);
}

void operator+=(float3& f0, const float& f1)
{
	f0 = float3(f0.x + f1, f0.y + f1, f0.z + f1);
}

void operator-=(float3& f0, const float& f1)
{
	f0 = float3(f0.x - f1, f0.y - f1, f0.z - f1);
}

void operator*=(float3& f0, const float& f1)
{
	f0 = float3(f0.x * f1, f0.y * f1, f0.z * f1);
}

void operator/=(float3& f0, const float& f1)
{
	f0 = float3(f0.x / f1, f0.y / f1, f0.z / f1);
}

void operator%=(float3& f0, const float& f1)
{
	f0 = float3(fmod(f0.x, f1), fmod(f0.y, f1), fmod(f0.z, f1));
}

float GetLength(const float3& fl)
{
	return std::sqrt(pow(fl.x, 2) + pow(fl.y, 2) + pow(fl.z, 2));
}

float GetLengthSquared(const float3& fl)
{
	return pow(fl.x, 2) + pow(fl.y, 2) + pow(fl.z, 2);
}

float3 fl3Normalize(const float3& fl)
{
	float len = GetLength(fl);

	return float3(fl.x / len, fl.y / len, fl.z / len);
}

float Dot(const float3& f0, const float3& f1)
{
	return (f0.x * f1.x, f0.y * f1.y, f0.z * f1.z);
}

float3 Cross(const float3& f0, const float3& f1)
{
	return float3
	(
		(f0.y * f1.z) - (f1.z * f0.y),
		(f0.z * f1.x) - (f1.x * f0.z),
		(f0.x * f1.y) - (f1.y * f0.x)
	);
}

float3 operator*(float3& fl, Mat& mat)
{
	auto vec = DirectX::XMLoadFloat3(&fl);
	vec = DirectX::XMVectorSetW(vec, 1.0f);

	auto val = XMVector3Transform(vec, mat);

	float3 res;
	DirectX::XMStoreFloat3(&res, val);
	return res;
}
