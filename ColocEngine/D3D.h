#pragma once
#include <cstdint>
#include <Windows.h>

#include<DirectXMath.h>
#include<dxgi1_4.h>
#include"MACRO.h"
#include"BUFFER.h"
#include<d3dcompiler.h>
#include "MeshLoader.h"
#include<DirectXTex.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3dcompiler.lib")

#pragma comment(lib,"assimp-vc143-mtd.lib")
using namespace DirectX;

//------------

class D3d
{
public:

	D3d();
	~D3d();

	bool Initialize(HWND hwnd , uint32_t h , uint32_t w);
	bool InitGBO();
	bool InitPSO();
	void Termination();
	void TermGBO();		//Graphic Buffer Object
	void Run(int interval);
	void Update();

	void SetHeight(float h);
	void SetWidth(float w);
	float GetHeight();
	float GetWidth();

	void SetColorBG(float R, float G, float B ,float A);

	ID3D12Device* GetDevice();
	ID3D12GraphicsCommandList* GetCMDList();

private:

	void write();
	void waitGPU();
	void present(uint32_t itv);

private:
	static const uint32_t FrameAmmount = 2;

	ID3D12Device* device_;
	IDXGISwapChain3* swpchain_;
	ID3D12Resource* colbuf_[FrameAmmount];
	ID3D12CommandQueue* cmdque_;
	ID3D12CommandAllocator* cmdalloc_[FrameAmmount];
	ID3D12GraphicsCommandList* cmdlist_;
	ID3D12DescriptorHeap* heapRTV_;

	ID3D12Fence* fence_;
	HANDLE event_fence;
	uint64_t fencecnt_[FrameAmmount];
	uint64_t IND_frame;
	D3D12_CPU_DESCRIPTOR_HANDLE h_RTV[FrameAmmount];
	D3D12_CPU_DESCRIPTOR_HANDLE h_ZBV;

	D3D12_RESOURCE_BARRIER brr;


	//-----------------
	ID3D12DescriptorHeap* heapCBV_SRV_UAV_;
	ID3D12DescriptorHeap* heapTex_;
	ID3D12DescriptorHeap* heapStructureBuffer_;
	ID3D12RootSignature* rootsig_;
	ID3D12PipelineState* PSO;

	ID3D12Resource* VB;
	ID3D12Resource* Ins_CB[FrameAmmount];
	ID3D12Resource* Cmn_CB[FrameAmmount];
	ID3D12Resource* GPUBuffer[FrameAmmount];
	ID3D12Resource* IB;
	ID3D12Resource* ZB;
	Texture			tex;

	D3D12_VERTEX_BUFFER_VIEW VBV;
	CBUFFERVIEW<ObjInfo> Ins_CBV[FrameAmmount];
	CBUFFERVIEW<Util>	 Cmn_CBV[FrameAmmount];
	D3D12_INDEX_BUFFER_VIEW IBV;
	ID3D12DescriptorHeap* hp_ZBV;

	//---------------
	D3D12_VIEWPORT view_;
	D3D12_RECT rect_;

	float angle_;

	//-----

	float Height;
	float Width;

	//-------
	vector<MESH> mesh_;
	vector<MATERIAL> mtr_;
	UINT meshcnt_;
	//-----
	float backcolor_ [4];
};

namespace PTR_D3D
{
	extern D3d* ptr;
}

namespace PTR_WND
{
	extern HWND* ptr;
}