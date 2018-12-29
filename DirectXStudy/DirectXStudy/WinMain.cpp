#include <Windows.h>

HWND g_hwnd;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Render(void);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPWSTR lpCmdLine, int cmdShowMode)
{
	//WindowClass
	WNDCLASSEX winClass;
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = hInstance;
	winClass.lpszClassName = L"DirectXStudyWinClass";
	winClass.lpszMenuName = nullptr;
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	
	winClass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	winClass.hIcon = nullptr;
	winClass.hIconSm = nullptr;
	winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

	winClass.lpfnWndProc = &WndProc;

	if (!RegisterClassEx(&winClass))
		return E_FAIL;
	//Window
	RECT clientRect = { 0, 0, 800, 600 };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);
	g_hwnd = CreateWindow(L"DirectXStudyWinClass", L"DirectStudy", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, nullptr, nullptr, hInstance, nullptr);

	if (!g_hwnd)
		return E_FAIL;

	ShowWindow(g_hwnd, cmdShowMode);
	MSG message = {0};
	//Message Loop
	while (WM_QUIT != message.message)
	{
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
			Render();
	}

	return (int)message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

void Render()
{

}