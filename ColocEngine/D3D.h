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
#include"Light.h"

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
	bool InitPost();
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
	void present(int itv);
	void postEffect();
	void render();

private:
	static const uint32_t FrameAmount = 2;

	ID3D12Device* device_;
	IDXGISwapChain3* swpchain_;
	ID3D12Resource* colbuf_[FrameAmount];
	ID3D12CommandQueue* cmdque_;
	ID3D12CommandAllocator* cmdalloc_[FrameAmount];
	ID3D12GraphicsCommandList* cmdlist_;
	ID3D12DescriptorHeap* heapRTV_;
	
	ID3D12Resource* post_;
	ID3D12DescriptorHeap* postRTV_;
	ID3D12DescriptorHeap* postSRV_;

	ID3D12Fence* fence_;
	HANDLE event_fence;
	uint64_t fencecnt_[FrameAmount];
	uint64_t IND_frame;
	D3D12_CPU_DESCRIPTOR_HANDLE h_RTV[FrameAmount];
	D3D12_CPU_DESCRIPTOR_HANDLE h_ZBV;

	D3D12_RESOURCE_BARRIER brr;

	//-----------------

	ID3D12RootSignature* rootsig_;
	ID3D12RootSignature* postRTSG_;

	ID3D12PipelineState* PSO;
	ID3D12PipelineState* postPSO;

	ID3D12Resource*				CB_Util[FrameAmount];
	ID3D12Resource*				CB_CAM[FrameAmount];
	ID3D12Resource*				CB_LGT[FrameAmount];

	ID3D12Resource* ZB;
	Texture			tex;

	StructuredBuffer<ObjInfo>	 SB_OI[FrameAmount];
	StructuredBuffer<MapBOOL>	 SB_MB[FrameAmount];
	StructuredBuffer<Material>	 SB_MTL[FrameAmount];
;
	CBUFFERVIEW<Util>			 CBV_Util[FrameAmount];
	CBUFFERVIEW<Cam>			 CBV_Cam[FrameAmount];
	struct CBUFFER_LIGHT
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
		D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE HGPU;

		uint16_t point_cnt;
		uint16_t dir_cnt;
		uint16_t amb_cnt;
		uint16_t area_cnt;
		uint16_t vol_cnt;
		uint16_t spot_cnt;
	}
								 CBV_LGT[FrameAmount];

	ID3D12DescriptorHeap* hp_ZBV;

	//---------------
	ID3D12Resource* postVB_;
	D3D12_VERTEX_BUFFER_VIEW postVBV_;
	ID3D12Resource* postIB_;
	D3D12_INDEX_BUFFER_VIEW postIBV_;

	D3D12_VIEWPORT view_;
	D3D12_RECT rect_;
	
	float angle_;

	//-----

	float Height;
	float Width;

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