#pragma once
#include"MACRO.h"
#include <dinput.h>
#include"Xinput.h"

#pragma comment(lib,"dInput8.lib")
#pragma comment(lib, "xinput.lib")

namespace Input_KB
{
	HRESULT Init();
	void Update();
	void Release();

	bool Down(int code);
	bool Free(int code);

	bool Push(int code);
	bool Up(int code);
};

namespace Input_CR
{
	HRESULT Init();
	void Update();
	void Release();

	bool Down(int index,int code);
	bool Free(int index,int code);

	bool Push(int index,int code);
	bool Up(int index,int code);

	float2 GetStick_Left(int index);
	float2 GetStick_Right(int index);

	bool isTrigger_Left(int index, float th);
	bool isTrigger_Right(int index, float th);

	void SetVib(int index, float left , float right);
};
