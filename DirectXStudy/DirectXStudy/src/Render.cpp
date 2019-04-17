#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "Render.h"
#include "Screen.h"

bool DXRender::_InitDevice()
{
	//Feature Level
	D3D_FEATURE_LEVEL featureLevel[] = {
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_0
	};

	UINT createDeviceFlag = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;

	D3D_FEATURE_LEVEL targetFeatureLevel;

	HRESULT result = E_FAIL;// D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevel, sizeof(featureLevel) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &g_d3dDevice, &targetFeatureLevel, &g_d3dDeviceContext);
	if (FAILED(result))
		return false;

	IDXGIDevice *dxgiDevice = nullptr;
	IDXGIAdapter *dxgiAdapter = nullptr;
	IDXGIFactory *dxgiFactory = nullptr;
	//result = g_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
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
	//swapChainDesc.OutputWindow = g_hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;

	//dxgiFactory->CreateSwapChain(g_d3dDevice, &swapChainDesc, &g_swapChain);
	//dxgiFactory->MakeWindowAssociation(g_hwnd, DXGI_MWA_NO_ALT_ENTER);
	dxgiFactory->Release();

	//Back Buffer as Render Target
	ID3D11Texture2D *backBuffer = nullptr;
	//result = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;
	//g_d3dDevice->CreateRenderTargetView(backBuffer, &rtvDesc, &g_rtView);
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
	//result = g_d3dDevice->CreateTexture2D(&tex2DDes, nullptr, &depthTex);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	//result = g_d3dDevice->CreateDepthStencilView(depthTex, &depthStencilViewDesc, &g_dsView);

	//g_d3dDeviceContext->OMSetRenderTargets(1, &g_rtView, nullptr);

	//View Port
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.Width = Screen::GetScreenWidth();
	viewport.Height = Screen::GetScreenHeight();
	//g_d3dDeviceContext->RSSetViewports(1, &viewport);

	//Basic Mesh
	//[QUESTION]深度???
	/*
	BasicVertex triangleMesh[] = {
		{ DirectX::XMFLOAT3(0, 0.5, 0), DirectX::XMFLOAT3(1, 0, 0) },
		{ DirectX::XMFLOAT3(0.5, -0.5, 0), DirectX::XMFLOAT3(0, 1, 0) },
		{ DirectX::XMFLOAT3(-0.5, -0.5, 0), DirectX::XMFLOAT3(0, 0, 1) }
	};
	*/

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	//bufferDesc.ByteWidth = sizeof(BasicVertex) * 3;

	D3D11_SUBRESOURCE_DATA bufferData = {};
	//bufferData.pSysMem = &triangleMesh;

	ID3D11Buffer *vertexBuffer;
	//g_d3dDevice->CreateBuffer(&bufferDesc, &bufferData, &vertexBuffer);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//Shader
	ID3DBlob *vertexShaderByteCode = nullptr;
	ID3DBlob *fragmentShaderByteCode = nullptr;
	ID3DBlob *errorMsg = nullptr;

	//[QUESTION]必须是hlsl后缀？
	//[QUESTION]尝试头文件包含?
	//[QUESTION]不能是VertexShader
	result = D3DCompileFromFile(L"vertex.hlsl", nullptr, nullptr, "Vertex", "vs_4_0", 0, 0, &vertexShaderByteCode, &errorMsg);
	if (FAILED(result))
	{
		if (errorMsg)
		{
			const char* err = (reinterpret_cast<const char*>(errorMsg->GetBufferPointer()));
			errorMsg->Release();
		}

		MessageBox(nullptr, L"Error", L"Error", MB_OK);
		return E_FAIL;
	}

	result = D3DCompileFromFile(L"fragment.hlsl", nullptr, nullptr, "FragmentShader", "ps_4_0", 0, 0, &fragmentShaderByteCode, &errorMsg);
	if (FAILED(result))
	{
		if (errorMsg)
		{
			const char* err = (reinterpret_cast<const char*>(errorMsg->GetBufferPointer()));
			errorMsg->Release();
		}
		return E_FAIL;
	}

	ID3D11VertexShader *vertexShader = nullptr;
	ID3D11PixelShader *fragmentShader = nullptr;
	//result = g_d3dDevice->CreateVertexShader(vertexShaderByteCode->GetBufferPointer(), vertexShaderByteCode->GetBufferSize(), nullptr, &vertexShader);
	//result = g_d3dDevice->CreatePixelShader(fragmentShaderByteCode->GetBufferPointer(), fragmentShaderByteCode->GetBufferSize(), nullptr, &fragmentShader);

	//g_d3dDeviceContext->VSSetShader(vertexShader, nullptr, 0);
	//g_d3dDeviceContext->PSSetShader(fragmentShader, nullptr, 0);

	//Input Layout
	//[QUESTION] 尝试shader里面位置是float4
	//[QUESTION] 尝试不匹配
	ID3D11InputLayout *inputLayout = nullptr;
	//result = g_d3dDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), vertexShaderByteCode->GetBufferPointer(), vertexShaderByteCode->GetBufferSize(), &inputLayout);

	//g_d3dDeviceContext->IASetInputLayout(inputLayout);

	//unsigned int stride = sizeof(BasicVertex);
	unsigned offset = 0;
	//g_d3dDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//g_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DXRender::_Render()
{
	//g_d3dDeviceContext->ClearRenderTargetView(g_rtView, DirectX::Colors::LightBlue);
	//g_d3dDeviceContext->Draw(3, 0);
	//g_swapChain->Present(0, 0);
}

void DXRender::_DestroyDevice()
{
	m_d3dDevice->Release();
	m_d3dDeviceContext->Release();
	m_swapChain->Release();

	//g_rtView->Release();
	//g_dsView->Release();

}

bool DXRender::InitDevice()
{
	if (m_instance != nullptr)
		return true;
	
	m_instance = new DXRender();
	m_instance->InitDevice();
	return false;
}

void DXRender::DestroyDevice()
{
	m_instance->DestroyDevice();
}

void DXRender::Render()
{
	m_instance->Render();
}