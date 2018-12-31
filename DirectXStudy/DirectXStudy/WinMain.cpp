#include <Windows.h>
#include <d3d11.h>
#include <DirectXColors.h>

//Global Variable
HWND					g_hwnd;
ID3D11Device			*g_d3dDevice;
ID3D11DeviceContext		*g_d3dDeviceContext;
IDXGISwapChain			*g_swapChain;
ID3D11RenderTargetView	*g_rtView;

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

	//Direct3D 11

	//Feature Level
	D3D_FEATURE_LEVEL featureLevel[] = {
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_0
	};

	UINT createDeviceFlag = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;

	D3D_FEATURE_LEVEL targetFeatureLevel;

	HRESULT result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevel, sizeof(featureLevel) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &g_d3dDevice, &targetFeatureLevel, &g_d3dDeviceContext);
	if (FAILED(result))
		return E_FAIL;

	IDXGIDevice *dxgiDevice = nullptr;
	IDXGIAdapter *dxgiAdapter = nullptr;
	IDXGIFactory *dxgiFactory = nullptr;
	result = g_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	result = dxgiDevice->GetAdapter(&dxgiAdapter);
	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));

	dxgiAdapter->Release();
	dxgiDevice->Release();

	GetClientRect(g_hwnd, &clientRect);
	//Swap Chain
	DXGI_SWAP_CHAIN_DESC scDes = {};
	scDes.BufferCount = 1;
	scDes.BufferDesc.Width = clientRect.right - clientRect.left;
	scDes.BufferDesc.Height = clientRect.bottom - clientRect.top;
	scDes.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
	scDes.BufferDesc.RefreshRate.Numerator = 60;
	scDes.BufferDesc.RefreshRate.Denominator = 1;
	scDes.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDes.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	scDes.OutputWindow = g_hwnd;
	scDes.SampleDesc.Count = 1;
	scDes.SampleDesc.Quality = 0;
	scDes.Windowed = true;
	
	result = dxgiFactory->CreateSwapChain(g_d3dDevice, &scDes, &g_swapChain);
	dxgiFactory->MakeWindowAssociation(g_hwnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	//Back Buffer as Render Target
	ID3D11Texture2D *backBuffer = nullptr;
	result = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

	result = g_d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &g_rtView);
	backBuffer->Release();

	g_d3dDeviceContext->OMSetRenderTargets(1, &g_rtView, nullptr);

	//View Port
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.Width = clientRect.right - clientRect.left;
	viewport.Height = clientRect.bottom - clientRect.top;
	g_d3dDeviceContext->RSSetViewports(1, &viewport);

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

	g_d3dDevice->Release();
	g_d3dDeviceContext->Release();
	g_swapChain->Release();
	g_rtView->Release();

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
	g_d3dDeviceContext->ClearRenderTargetView(g_rtView, DirectX::Colors::LightBlue);
	g_swapChain->Present(0, 0);
}