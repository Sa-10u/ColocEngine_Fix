#pragma once
#include <Windows.h>
#include <cstdint>
#include"D3D.h"
#include"GameMain.h"
#include"IComp.h"
#include"WorldManager.h"
#include"Input.h"
#include<chrono>

#include<DirectXMath.h>
#include<d3d12.h>
#include<dxgi1_4.h>

using namespace::DirectX;


class WinView
{
public:

	WinView(uint32_t h, uint32_t w);
	~WinView();
	void Run();
	HWND GetWindowHandle();
	
private:

	HINSTANCE h_ins;
	HWND h_wnd;
	uint32_t h_;
	uint32_t w_;

	D3d* D3D;
	float fps;

	const uint16_t FPS;

private:

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	bool setup();
	bool initialize();
	void termination();
	void loop();

	void ChangeAspect();
};

namespace WND_NAME
{
	const auto smp = TEXT("Sample");
}

