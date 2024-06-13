#include "Input.h"
#include "WinView.h"
#pragma warning(disable : 4995)
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
   constexpr uint8_t MAX_Amount = 4;
   // uint8_t Using_Amount = 0;
   // LPDIRECTINPUTDEVICE8 CTR[MAX_Amount] = {};
   // DIJOYSTATE2 state_[MAX_Amount];
   // DIJOYSTATE2 prev_state[MAX_Amount];

   XINPUT_STATE state_[MAX_Amount];
   XINPUT_STATE before_[MAX_Amount];

    HRESULT Init()
    {
        /*
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
        */

        XInputEnable(true);
        for (auto i = 0u; i < MAX_Amount; ++i) {

            memset(&state_[i], 0, sizeof(XINPUT_STATE));
        }

        return S_OK;
    }

    void Update()
    {
        /*
        for (auto i = 0u; i < Using_Amount; i++) {
            
            memcpy(&prev_state[i], &state[i], sizeof(state[i]));
            CTR[i]->Acquire();
            CTR[i]->GetDeviceState(sizeof(CTR[i]), &CTR[i]);
        }
        */

        memcpy(&before_, &state_, sizeof(XINPUT_STATE) * MAX_Amount);

        for (auto i = 0u; i < MAX_Amount; ++i) {

            XInputGetState(i, &state_[i]);
        }
    }

    void Release()
    {
        if (DInput != nullptr)
        {
            DInput->Release();
            DInput = nullptr;
        }
        /*
        for (auto i = 0u; i < MAX_Amount; ++i) {
            if (CTR[i] != nullptr)
            {
                CTR[i]->Unacquire();
                CTR[i]->Release();
                CTR[i]= nullptr;
            }
        }
        */
        XInputEnable(false);
    }

    bool Down(int index, int code)
    {
        return state_[index].Gamepad.wButtons & code;
    }

    bool Free(int index, int code)
    {
        return !(state_[index].Gamepad.wButtons & code);
    }

    bool Push(int index, int code)
    {
        return false;
    }

    bool Up(int index, int code)
    {
        return false;
    }

    float2 GetStick_Left(int index)
    {
        //-32768 to 32767
        auto&& x = state_[index].Gamepad.sThumbLX;
        auto&& y = state_[index].Gamepad.sThumbLY;

        return float2
        {
            x > 0 ? x / 32767.0f : x / 32768.0f,
            y > 0 ? y / 32767.0f : y / 32768.0f
        };
    }

    float2 GetStick_Right(int index)
    {
        //32768 - 32767
        auto&& x = state_[index].Gamepad.sThumbRX;
        auto&& y = state_[index].Gamepad.sThumbRY;

        return float2
        {
            x > 0 ? x / 32767.0f : x / 32768.0f,
            y > 0 ? y / 32767.0f : y / 32768.0f
        };
    }

    bool isTrigger_Left(int index, float th)
    {
        return state_[index].Gamepad.bLeftTrigger > th * 0xff;
    }

    bool isTrigger_Right(int index, float th)
    {
        return state_[index].Gamepad.bRightTrigger > th * 0xff;
    }

    void SetVib(int index, float left, float right)
    {
        float l = (std::min)(left, 100.0f);
        float r = (std::min)(right, 100.0f);

        XINPUT_VIBRATION v = { (l * UINT16_MAX),(r * UINT16_MAX) }; 

        XInputSetState(index, &v);
    }
}