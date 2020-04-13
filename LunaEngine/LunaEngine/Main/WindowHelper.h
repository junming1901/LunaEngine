#pragma once
#include <Windows.h>

#define MAX_LOADSTRING 100

// Singleton
class Window
{
public:

	bool Initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow);

	static Window* GetInstance();

	friend int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
								 _In_opt_ HINSTANCE hPrevInstance,
							     _In_ LPWSTR lpCmdLine,
								 _In_ int nCmdShow);
	
private:

	Window();

	static void CreateInstance(const char* title, const char* window_name);

	ATOM RegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void RecieveMessage();

	MSG GetMsg();

	// Variables
	static Window* m_Instance;

	static HINSTANCE m_hInst;
	
	WCHAR m_Title[MAX_LOADSTRING];
	WCHAR m_WindowClass[MAX_LOADSTRING];

	HACCEL m_AccelTable;
	MSG m_Msg;

};