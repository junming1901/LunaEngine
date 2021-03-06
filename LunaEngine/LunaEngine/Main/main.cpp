// LunaEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WindowHelper.h"
#include "LunaEngine.h"
#include <fstream>
#include <iostream>

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

    LunaEngine::CreateInstance();

    LunaEngine* engine = LunaEngine::GetInstance();

    engine->Init();

    std::cout << "Initialization Complete..." << std::endl;

    do
    {
        window->RecieveMessage();

    } while (engine->Update());

    engine->Free();

    LunaEngine::DeleteInstance();
    
    int param = (int)window->GetMsg().wParam;

    Window::DeleteInstance();

    return param;
}


