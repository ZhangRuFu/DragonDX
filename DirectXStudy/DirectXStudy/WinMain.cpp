#include <Windows.h>

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
	
	winClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	

	//Window

	//Message Loop
}