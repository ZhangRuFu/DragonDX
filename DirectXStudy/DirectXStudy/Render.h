#pragma once
#include <d3d11.h>

class DXRender
{
private:
	static DXRender *m_instance;

private:
	ID3D11Device *m_d3dDevice;
	ID3D11DeviceContext *m_d3dDeviceContext;
	IDXGISwapChain *m_swapChain;
	ID3D11RenderTargetView *m_rtView;

private:
	bool _InitDevice();
	void _Render();
	void _DestroyDevice();

public:
	static bool InitDevice();
	static void Render();
	static void DestroyDevice();
	
};