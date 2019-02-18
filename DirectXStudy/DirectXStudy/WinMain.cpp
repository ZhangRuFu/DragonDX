#include <Windows.h>
#include <d3d11.h>
#include <DirectXColors.h>

#include "Screen.h"

//Global Variable
HWND					g_hwnd;
ID3D11Device			*g_d3dDevice;
ID3D11DeviceContext		*g_d3dDeviceContext;
IDXGISwapChain			*g_swapChain;
ID3D11RenderTargetView	*g_rtView;
ID3D11DepthStencilView	*g_dsView;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Render(void);

void InitEngine(void);
void InitWindow(void);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPWSTR lpCmdLine, int cmdShowMode)
{
	InitEngine();

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

	RECT clientRect = { 0, 0, Screen::GetScreenWidth(), Screen::GetScreenHeight() };
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

	//Swap Chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = Screen::GetScreenWidth();
	swapChainDesc.BufferDesc.Height = Screen::GetScreenHeight();
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = g_hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;

	dxgiFactory->CreateSwapChain(g_d3dDevice, &swapChainDesc, &g_swapChain);
	dxgiFactory->MakeWindowAssociation(g_hwnd, DXGI_MWA_NO_ALT_ENTER);
	dxgiFactory->Release();

	//Back Buffer as Render Target
	ID3D11Texture2D *backBuffer = nullptr;
	result = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;
	g_d3dDevice->CreateRenderTargetView(backBuffer, &rtvDesc, &g_rtView);
	backBuffer->Release();

	//Depth Stencil Buffer
	D3D11_TEXTURE2D_DESC tex2DDes;
	tex2DDes.Width = Screen::GetScreenWidth();
	tex2DDes.Height = Screen::GetScreenHeight();
	tex2DDes.ArraySize = 1;
	tex2DDes.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	tex2DDes.CPUAccessFlags = 0;
	tex2DDes.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	tex2DDes.MipLevels = 1;
	tex2DDes.MiscFlags = 0;
	tex2DDes.SampleDesc.Count = 1;
	tex2DDes.SampleDesc.Quality = 0;
	tex2DDes.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	ID3D11Texture2D *depthTex = nullptr;
	result = g_d3dDevice->CreateTexture2D(&tex2DDes, nullptr, &depthTex);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	result = g_d3dDevice->CreateDepthStencilView(depthTex, &depthStencilViewDesc, &g_dsView);

	g_d3dDeviceContext->OMSetRenderTargets(1, &g_rtView, g_dsView);

	//View Port
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.Width = Screen::GetScreenWidth();
	viewport.Height = Screen::GetScreenHeight();
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
	g_dsView->Release();

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

void InitEngine(void)
{
	Screen::Init(800, 600);
}

void Render()
{
	g_d3dDeviceContext->ClearRenderTargetView(g_rtView, DirectX::Colors::LightBlue);
	g_swapChain->Present(0, 0);
}