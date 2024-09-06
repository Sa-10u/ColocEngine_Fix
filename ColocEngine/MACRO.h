#pragma once
#include<DirectXMath.h>
#include <windows.h>
#include<d3d12.h>

template<typename tgt>
inline void SAFE_DELETE(tgt*& p)
{
	if (!(p == nullptr))
	{
		delete p;
		p = nullptr;
	}
}

template<typename tgt>
inline void SAFE_RELEASE(tgt*& p)
{
	if (!(p == nullptr))
	{
		p->Release();
		p = nullptr;
	}

}

inline float GetGradation(float min,float max, float val)
{
	int&& r = max - min;
	int&& m = val - min;

	return static_cast<float>(m) / r;
}

#define __guidof __uuidof

#define _DebugCUI true;

typedef DirectX::XMFLOAT2 float2;
typedef DirectX::XMFLOAT3 float3;
typedef DirectX::XMFLOAT4 float4;

typedef DirectX::XMMATRIX Mat;
typedef DirectX::XMVECTOR Vec;

namespace SEMANTICS_STR
{
	extern LPCSTR POSITION;
	extern LPCSTR COLOR;
	extern LPCSTR TEXCOORD;
	extern LPCSTR TANGENT;
	extern LPCSTR NORMAL;
	extern LPCSTR BITANGENT;
	extern LPCSTR MATERIAL;
	extern LPCSTR BONEIDs;
	extern LPCSTR BONEWEIGHTs;
}
namespace SHADER_FILENAME
{
	extern LPCWSTR DefDeferredVS;
	extern LPCWSTR DefDeferredPS;

	extern LPCWSTR DefVS;
	extern LPCWSTR DefPS;

	extern LPCWSTR DefPostVS;
	extern LPCWSTR DefPostPS;

	extern LPCWSTR UI_VS;
	extern LPCWSTR UI_PS;
}

char* wtoc(const wchar_t* str);
wchar_t* ctow(const char* str);

class DH
{
public:
	DH() = delete;
	DH(const DH& dh) = delete;
	DH(const DH&& dh) = delete;
	DH(UINT incre, ID3D12DescriptorHeap** pheap);

	D3D12_CPU_DESCRIPTOR_HANDLE GetAndIncreCPU();
	D3D12_GPU_DESCRIPTOR_HANDLE GetAndIncreGPU();
	void ResetHandles(D3D12_CPU_DESCRIPTOR_HANDLE h_cpu, D3D12_GPU_DESCRIPTOR_HANDLE h_gpu);
	void ResetIncre(UINT incre);

	ID3D12DescriptorHeap** ppHeap_;

	~DH();

private:

	UINT incre_;
	D3D12_CPU_DESCRIPTOR_HANDLE h_cpu;
	D3D12_GPU_DESCRIPTOR_HANDLE h_gpu;

};

float3 operator+(const float3& f0, const float3& f1);
float3 operator-(const float3& f0, const float3& f1);
float3 operator*(const float3& f0, const float3& f1);
float3 operator/(const float3& f0, const float3& f1);
float3 operator%(const float3& f0, const float3& f1);
bool operator==(const float3& f0, const float3& f1);
bool operator!=(const float3& f0, const float3& f1);
float3 operator++(float3& f0);
float3 operator++(float3& f0 ,int dummy);
float3 operator--(float3& f0);
float3 operator--(float3& f0, int dummy);
void operator+=(float3& f0, const float3& f1);
void operator-=(float3& f0, const float3& f1);
void operator*=(float3& f0, const float3& f1);
void operator/=(float3& f0, const float3& f1);
void operator%=(float3& f0, const float3& f1);

float3 operator+(const float3& f0, const float& f1);
float3 operator-(const float3& f0, const float& f1);
float3 operator*(const float3& f0, const float& f1);
float3 operator/(const float3& f0, const float& f1);
float3 operator%(const float3& f0, const float& f1);
float3 operator==(const float3& f0, const float& f1);
float3 operator!=(const float3& f0, const float& f1);
void operator+=(float3& f0, const float& f1);
void operator-=(float3& f0, const float& f1);
void operator*=(float3& f0, const float& f1);
void operator/=(float3& f0, const float& f1);
void operator%=(float3& f0, const float& f1);
float GetLength(const float3& fl);
float GetLengthSquared(const float3& fl);
float3 fl3Normalize(const float3& fl);
float Dot(const float3& f0, const float3& f1);
float3 Cross(const float3& f0, const float3& f1);

float3 operator*(float3& fl, Mat& mat);

#define __CREATE(LPCSTR)	{};