#include "WinView.h"
#include <chrono>
#include"CAM.h"
#include"FileLoader.h"
#include<thread>
#include"ResourceManager.h"
#include"PSOManager.h"
#include"ShaderCompiler.h"
#include"S_Sound.h"

using namespace std::chrono;

#define _ALL NULL
#define _THIS NULL

//typedef floatMS = chrono::duration

WinView::WinView(uint32_t h, uint32_t w) :h_ins(nullptr), h_wnd(nullptr), h_(h), w_(w),D3D(nullptr),FPS(60)
{
}

WinView::~WinView()
{
}

void WinView::Run()
{
    if (setup())
    {
        loop();
    }

    termination();
}

HWND WinView::GetWindowHandle()
{
    return h_wnd;
}

//-------------------------------------------------------------------------------------

bool WinView::setup()
{
    D3D = new D3d;

    if (initialize())
    {
        if (D3D->Initialize(h_wnd,h_,w_))
        {
            
            ResourceManager::Init();
            CAM::Init();
            DataManager::Init();
            WorldManager::Init();
            Input_KB::Init();
            S_Sound::Init();

            GameMain::Init();

            return true;
        }
    }
    
    return false;
}

bool WinView::initialize()
{
    auto Frame =60.0f;
    fps = 1000.0f / Frame;

    constexpr bool FAIL = 0;

    WNDCLASSEX wcex = {};

    h_ins = GetModuleHandle(_THIS);  

    if (h_ins == nullptr) return FAIL;

    {
        auto color = CreateSolidBrush(RGB(1, 10, 100));
        std::wstring str = L"CE_Icon.ico";
        
        wcex.lpfnWndProc = WndProc;
        wcex.lpszClassName = WND_NAME::smp;
        wcex.lpszMenuName = nullptr;
        wcex.cbSize = sizeof(wcex);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.hIcon = LoadIcon(h_ins, IDI_QUESTION);
        wcex.hIconSm = LoadIcon(h_ins, IDI_EXCLAMATION);
        wcex.hCursor = LoadCursor(h_ins, IDC_ARROW);
        wcex.hInstance = h_ins;
        wcex.hbrBackground = color;

        color = nullptr;
    }
    auto rgc = RegisterClassEx(&wcex);

    if(!rgc) return FAIL;

    RECT box = { 0,0,0,0 };
    {
        box.right = static_cast<LONG>(w_);
        box.bottom = static_cast<LONG>(h_);
    }

    {
        auto style = WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU;
        AdjustWindowRect(&box, style, false);

        h_wnd = CreateWindowEx
        (
            NULL,
            WND_NAME::smp,
            WND_NAME::smp,
            style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            (box.right - box.left),
            (box.bottom - box.top),
            nullptr,
            nullptr,
            h_ins,
            0
        );
    }

    if (h_wnd == nullptr)    return FAIL;

    ShowWindow(h_wnd,SW_SHOWNORMAL);
    UpdateWindow(h_wnd);
    SetFocus(h_wnd);

    return true;
}

void WinView::termination()
{

    if (h_ins != nullptr)
    {
        D3D->Termination();
        delete D3D;
        UnregisterClass(WND_NAME::smp, h_ins);
    }

    h_ins = nullptr;
    h_wnd = nullptr;

    GameMain::Release();
    Input_KB::Release();
    WorldManager::Release();
    DataManager::Release();
    
    PSOManager::Term();
    ResourceManager::Term();
    S_Sound::Term();
}

void WinView::loop()
{
    MSG msg = {};

    auto st = system_clock::now();
    auto ed = system_clock::now();
    float tick = .0f;

    while (msg.message != WM_QUIT) {

        if (PeekMessage(&msg, _ALL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else
        {
            st = system_clock::now();

            Input_KB::Update();
            GameMain::Update(tick);
            D3D->Run(1);
            WorldManager::Changer();

            S_Sound::Run();

            ed = system_clock::now();
            auto&& t = ed - st;
            auto mili = duration_cast<milliseconds>(t).count();

            auto lim = static_cast<int64_t>(fps - mili);
    
            std::this_thread::sleep_for(std::chrono::milliseconds(lim));
            
            tick = lim;

            SetWindowText(h_wnd, std::to_wstring(lim).c_str());
        }
    }
}

void WinView::ChangeAspect()
{
    D3D->SetHeight(h_);
    D3D->SetWidth(w_);
}


LRESULT WinView::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_DESTROY:PostQuitMessage(0);     return 0;

    default: return DefWindowProc(hwnd,msg,wp,lp);

    }
}