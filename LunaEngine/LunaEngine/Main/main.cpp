// LunaEngine.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowHelper.h"
#include <fstream>

#define CREATE_CONSOLE true

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#if CREATE_CONSOLE
    if (AllocConsole())
    {
        FILE* file;

        freopen_s(&file, "CONOUT$", "wt", stdout);
        freopen_s(&file, "CONOUT$", "wt", stderr);
        freopen_s(&file, "CONOUT$", "wt", stdin);

        printf("Luna Engine Starting...\n");
    }
#endif

    Window::CreateInstance("Luna Engine", "Luna Engine");

    Window* window = Window::GetInstance();

    if (window->Initialize(hInstance, nCmdShow) == false)
    {
        return false;
    }

    window->RecieveMessage();

    return (int)window->GetMsg().wParam;
}


