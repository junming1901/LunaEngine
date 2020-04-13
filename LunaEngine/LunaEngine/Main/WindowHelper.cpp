#include "stdafx.h"
#include "Resource.h"
#include "WindowHelper.h"

Window* Window::m_Instance = nullptr;
HINSTANCE Window::m_hInst;

Window::Window()
: m_Msg {}, m_Title {}, m_WindowClass {}
{
}

bool Window::Initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow)
{
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, m_Title, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LUNAENGINE, m_WindowClass, MAX_LOADSTRING);
    RegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return false;
    }

    m_AccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LUNAENGINE));

    return true;
}

Window* Window::GetInstance()
{
    return m_Instance;
}

void Window::CreateInstance(const char* title, const char* window_name)
{
	m_Instance = new Window;

    strcpy_s(reinterpret_cast<char*>(m_Instance->m_Title), MAX_LOADSTRING, title);
    strcpy_s(reinterpret_cast<char*>(m_Instance->m_WindowClass), MAX_LOADSTRING, window_name);
}

void Window::DeleteInstance()
{
    delete m_Instance;
}

ATOM Window::RegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LUNAENGINE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = m_WindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL Window::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    m_hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(m_WindowClass, m_Title, WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            UNREFERENCED_PARAMETER(wmId);
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            UNREFERENCED_PARAMETER(hdc);

            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void Window::RecieveMessage()
{
    // Main message loop:
    while (GetMessage(&m_Msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(m_Msg.hwnd, m_AccelTable, &m_Msg))
        {
            TranslateMessage(&m_Msg);
            DispatchMessage(&m_Msg);
        }
    }
}

MSG Window::GetMsg()
{
    return m_Msg;
}
