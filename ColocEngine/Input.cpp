#include "Input.h"
#include "WinView.h"

namespace Input_KB
{
    LPDIRECTINPUT8   DInput = nullptr;
    LPDIRECTINPUTDEVICE8 KB = nullptr;
    char KBS[256] = {};
    char prev_KBS[256] = {};

    HRESULT Input_KB::Init()
    {
        HRESULT res;

        res = DirectInput8Create
        (
            GetModuleHandle(NULL),
            DIRECTINPUT_VERSION,
            IID_IDirectInput8,
            reinterpret_cast<void**>(&DInput),
            nullptr
        );
        if (FAILED(res)) return E_FAIL;

        res = DInput->CreateDevice
        (
            GUID_SysKeyboard,
            &KB,
            nullptr
        );
        if (FAILED(res)) return E_FAIL;

        res = KB->SetDataFormat
        (
            &c_dfDIKeyboard
        );
        if (FAILED(res)) return E_FAIL;

        res = KB->SetCooperativeLevel
        (
            *PTR_WND::ptr,
            DISCL_BACKGROUND | DISCL_NONEXCLUSIVE
        );
        if (FAILED(res)) return E_FAIL;

        res = KB->Acquire();
        if (FAILED(res)) return E_FAIL;
    }

    void Input_KB::Update()
    {
        memcpy(prev_KBS, KBS, sizeof(KBS));
        KB->Acquire();
        KB->GetDeviceState(sizeof(KBS), &KBS);
    }

    void Input_KB::Release()
    {
        if (DInput != nullptr)
        {
            if (KB != nullptr)
            {
                KB->Unacquire();
                KB->Release();
                KB = nullptr;
            }

            DInput->Release();
            DInput = nullptr;
        }
    }

    bool Input_KB::Down(int code)
    {
        return static_cast<bool>((KBS[code] & 0x80));
    }

    bool Input_KB::Free(int code)
    {
        return static_cast<bool>(~(KBS[code] & 0x80));
    }

    bool Input_KB::Push(int code)
    {
        return static_cast<bool>((KBS[code] & 0x80) & (~prev_KBS[code] & 0x80));
    }

    bool Input_KB::Up(int code)
    {
        return static_cast<bool>(~(KBS[code] & 0x80) & (prev_KBS[code] & 0x80));
    }

}