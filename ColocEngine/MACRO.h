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

#define __guidof __uuidof

constexpr double _PI = 3.141592;


typedef DirectX::XMFLOAT2 fl2;
typedef DirectX::XMFLOAT3 fl3;
typedef DirectX::XMFLOAT4 fl4;

typedef DirectX::XMMATRIX Mat;
typedef DirectX::XMVECTOR Vec;

namespace SEMANTICS_STR
{
	extern LPCSTR POSITION ;
	extern LPCSTR COLOR;
	extern LPCSTR TEXCOORD;
	extern LPCSTR TANGENT;
	extern LPCSTR NORMAL;
	extern LPCSTR BITANGENT;
	extern LPCSTR MATERIAL;
}
namespace SHADER_FILENAME
{
	extern LPCWSTR PoorVS;
	extern LPCWSTR PoorPS;

	extern LPCWSTR LambertPS;
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


#define __CREATE(LPCSTR)	{};