#include "Input.h"
#include "WinView.h"

LPDIRECTINPUT8   DInput = nullptr;

namespace Input_KB
{
    LPDIRECTINPUTDEVICE8 KB = nullptr;
    char KBS[256] = {};
    char prev_KBS[256] = {};

    HRESULT Input_KB::Init()
    {
        HRESULT res;

        //------------------------------
        if (DInput == nullptr)
        {
            res = DirectInput8Create
            (
                GetModuleHandle(NULL),
                DIRECTINPUT_VERSION,
                IID_IDirectInput8,
                reinterpret_cast<void**>(&DInput),
                nullptr
            );
            if (FAILED(res)) return E_FAIL;
        }
        //------------------------------

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
            DInput->Release();
            DInput = nullptr;
        }

        if (KB != nullptr)
        {
            KB->Unacquire();
            KB->Release();
            KB = nullptr;
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

namespace Input_CR
{
    constexpr uint8_t MAX_Amount = 1;
    uint8_t Using_Amount = 0;
    LPDIRECTINPUTDEVICE8 CTR[MAX_Amount] = {};
    DIJOYSTATE2 state[MAX_Amount];
    DIJOYSTATE2 prev_state[MAX_Amount];

    HRESULT Init()
    {
        HRESULT res;

        //------------------------------
        if (DInput == nullptr)
        {
            res = DirectInput8Create
            (
                GetModuleHandle(NULL),
                DIRECTINPUT_VERSION,
                IID_IDirectInput8,
                reinterpret_cast<void**>(&DInput),
                nullptr
            );
            if (FAILED(res)) return E_FAIL;
        }
        //------------------------------

        auto PASCAL callback = [](LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)->BOOL 
            {
                HRESULT res;

                res = DInput->CreateDevice
                (
                    lpddi->guidInstance,
                    &CTR[Using_Amount],
                    NULL
                );
                if (FAILED(res)) return DIENUM_STOP;

                {
                    CTR[Using_Amount]->SetDataFormat(&c_dfDIJoystick2);
                    CTR[Using_Amount]->SetCooperativeLevel(*PTR_WND::ptr, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

                    res = CTR[Using_Amount]->Acquire();
                    if (FAILED(res)) return E_FAIL;
                }

                Using_Amount++;

                return Using_Amount >= MAX_Amount ? DIENUM_STOP : DIENUM_CONTINUE;
            };

        res = DInput->EnumDevices
        (
            DI8DEVCLASS_GAMECTRL,
            callback,
            NULL,
            DIEDFL_FORCEFEEDBACK
        );
    }

    void Update()
    {
        for (auto i = 0u; i < Using_Amount; i++) {
            
            memcpy(&prev_state[i], &state[i], sizeof(state[i]));
            CTR[i]->Acquire();
            CTR[i]->GetDeviceState(sizeof(CTR[i]), &CTR[i]);
        }
    }

    void Release()
    {
        if (DInput != nullptr)
        {
            DInput->Release();
            DInput = nullptr;
        }

        for (auto i = 0u; i < MAX_Amount; ++i) {
            if (CTR[i] != nullptr)
            {
                CTR[i]->Unacquire();
                CTR[i]->Release();
                CTR[i]= nullptr;
            }
        }
    }

    bool Down(int code)
    {
        return false;
    }
    bool Free(int code)
    {
        return false;
    }
    bool Push(int code)
    {
        return false;
    }
    bool Up(int code)
    {
        return false;
    }
}