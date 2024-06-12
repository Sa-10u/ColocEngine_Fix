#pragma once
#include <dinput.h>
#pragma comment(lib,"dInput8.lib")

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

	bool Down(int code);
	bool Free(int code);

	bool Push(int code);
	bool Up(int code);
};
