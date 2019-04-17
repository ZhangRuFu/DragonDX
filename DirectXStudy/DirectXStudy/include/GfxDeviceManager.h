#pragma once
#include <dxgi.h>
#include <vector>
#include "String.h"
class GfxDeviceManager
{
private:
	static GfxDeviceManager *m_instance;

	IDXGIFactory *m_dxgiFactory;
	std::vector<IDXGIAdapter*> m_adapters;
	IDXGIAdapter* m_curAdapter;

private:
	GfxDeviceManager();
	~GfxDeviceManager();

public:
	static void Init(void);
	static void Destroy(void);
	static GfxDeviceManager* Instance(void) { return m_instance; }

	std::vector<String> GetAllDeviceDescription(void);
	bool CreateDevice(unsigned int index);
};