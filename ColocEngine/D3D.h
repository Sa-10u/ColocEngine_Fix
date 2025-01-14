#pragma once
#include <cstdint>
#include <Windows.h>

#include<DirectXMath.h>
#include<dxgi1_4.h>
#include"MACRO.h"
#include<d3dcompiler.h>
#include "MeshLoader.h"
#include<DirectXTex.h>

#include"C_UI.h"
#include"S_Light.h"

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

	bool Initialize(HWND hwnd, uint32_t h, uint32_t w);
	bool InitGBO();
	bool InitPost();
	bool InitText();
	void Termination();
	void TermGBO();		//Graphic Buffer Object
	void Run(int interval);
	void Update();

	void SetHeight(float h);
	void SetWidth(float w);
	float GetHeight();
	float GetWidth();

	void SetColorBG(float R, float G, float B, float A);

	ID3D12Device* GetDevice();
	ID3D12GraphicsCommandList* GetCMDList();

private:

	void GPGPUSkinning();
	void write();
	void waitGPU();
	void deferredrender();
	void present(int itv);
	void postEffect();
	void finalrender();
	void preeffectUI();
	void constantUI();

private:
	static const uint32_t FrameAmount = 2;

	ID3D12Device* device_;
	IDXGISwapChain3* swpchain_;
	ID3D12Resource* colbufRTV_[FrameAmount];
	ID3D12CommandQueue* cmdque_;
	ID3D12CommandAllocator* cmdalloc_[FrameAmount];
	ID3D12GraphicsCommandList* cmdlist_;
	ID3D12DescriptorHeap* colbuf_;

	ID3D12Resource* preRTV_[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D12DescriptorHeap* pre_;

	ID3D12Resource* firstpathRTV_;
	ID3D12DescriptorHeap* firstpath_;
	D3D12_CPU_DESCRIPTOR_HANDLE f_CPU_SRV;
	D3D12_GPU_DESCRIPTOR_HANDLE f_GPU_SRV;

	ID3D12Fence* fence_;
	HANDLE event_fence;
	uint64_t fencecnt_[FrameAmount];
	uint64_t IND_frame;
	D3D12_CPU_DESCRIPTOR_HANDLE h_RTV[FrameAmount];
	D3D12_CPU_DESCRIPTOR_HANDLE h_ZBV;

	D3D12_RESOURCE_BARRIER brr[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];

	//-----------------

	ID3D12Resource*						CB_Util[FrameAmount];
	ID3D12Resource*						CB_CAM[FrameAmount];
	ID3D12Resource*						CB_LGT[FrameAmount];
	ID3D12Resource*						CB_OTHERS[FrameAmount];

	ID3D12Resource* ZB;
	Texture			tex;

	StructuredBuffer<ObjInfo>			SB_OI[FrameAmount];
	StructuredBuffer<MapBOOL>			SB_MB[FrameAmount];
	StructuredBuffer<Material>			SB_MTL[FrameAmount];

	StructuredBuffer<SimpleQuad>		SB_UI[FrameAmount];
	
	CBUFFERVIEW<Util>					CBV_Util[FrameAmount];
	CBUFFERVIEW<Cam>					CBV_Cam[FrameAmount];
	CBUFFERVIEW<Lights>					CBV_LGT[FrameAmount];

	ID3D12DescriptorHeap* hp_ZBV;

	D3D12_CPU_DESCRIPTOR_HANDLE h_CPU_SRV[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
	D3D12_GPU_DESCRIPTOR_HANDLE h_GPU_SRV[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
	D3D12_CPU_DESCRIPTOR_HANDLE h_preRTV[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];

	//---------------
	ID3D12Resource* quadVB_;
	D3D12_VERTEX_BUFFER_VIEW quadVBV_;

	D3D12_VIEWPORT view_;
	D3D12_RECT rect_;

	float angle_;

	//-----

	float Height;
	float Width;

	//-----
	float backcolor_[4];
	float zerocolor_[4];

	//------------

	public:
	enum class RenderUsage
	{
		Color = 0,
		Normal,
		Emission,
		Depth,
		Position,
		t0,
		t1,
		t2,

		AMOUNT = D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT
	};
};

namespace PTR_D3D
{
	extern D3d* ptr;
}

namespace PTR_WND
{
	extern HWND* ptr;
}