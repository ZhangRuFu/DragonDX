
#include <Windows.h>
#include <string>
#include "Screen.h"
#include "Console.h"

HWND g_hwnd;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPWSTR cmdLine, int nCmdShow)
{
	//Window Class
	WNDCLASSEXW wndClassEX = {};
	wndClassEX.cbClsExtra = 0;
	wndClassEX.cbWndExtra = 0;
	wndClassEX.cbSize = sizeof(WNDCLASSEXW);
	wndClassEX.hInstance = hInstance;
	wndClassEX.lpszClassName = L"MainWindowClass";
	wndClassEX.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndClassEX.hbrBackground = nullptr;
	wndClassEX.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wndClassEX.hIcon = nullptr;
	wndClassEX.hIconSm = nullptr;
	wndClassEX.lpfnWndProc = WndProc;

	if (!RegisterClassExW(&wndClassEX))
		return E_FAIL;

	Screen::Init(800, 800);
	RECT rect = { 0, 0, Screen::GetScreenWidth(), Screen::GetScreenHeight() };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	g_hwnd = CreateWindowW(L"MainWindowClass", L"DirectXStudy", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom, nullptr, nullptr, hInstance, nullptr);
	if (g_hwnd == nullptr)
		return E_FAIL;

	ShowWindow(g_hwnd, nCmdShow);

	MSG message = {};
	while(WM_QUIT != message.message)
	{
		if(PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			DispatchMessage(&message);
	}

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_ERASEBKGND:
		//Do Nothing
		return 1;
		break;
	case WM_DESTROY:
		Console::Destroy();

		PostQuitMessage(0);
		break;
	case WM_PAINT:
		ValidateRect(hwnd, nullptr);
		break;
	case WM_CREATE:
		Console::Init();
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}
