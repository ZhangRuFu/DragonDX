#include "GfxDeviceManager.h"
#include "Format.h"

GfxDeviceManager::GfxDeviceManager()
{
	int curAdpterIndex = 0;
	IDXGIAdapter *curAdapter = nullptr;
	HRESULT enumRes = S_OK;
	while (enumRes == S_OK)
	{
		enumRes = m_dxgiFactory->EnumAdapters(curAdpterIndex, &curAdapter);
		m_adapters.push_back(curAdapter);
	}
}

GfxDeviceManager::~GfxDeviceManager()
{
	m_dxgiFactory->Release();
	m_dxgiFactory = nullptr;

}

void GfxDeviceManager::Init()
{
	if (m_instance == nullptr)
		m_instance = new GfxDeviceManager();
}

void GfxDeviceManager::Destroy()
{
	delete m_instance;
	m_instance = nullptr;
}

std::vector<String> GfxDeviceManager::GetAllDeviceDescription(void)
{
	std::vector<String> allDeviceDescs;
	DXGI_ADAPTER_DESC adapterDesc;
	for (unsigned int i = 0; i < m_adapters.size(); ++i)
	{
		IDXGIAdapter *curAdapter = m_adapters[i];
		curAdapter->GetDesc(&adapterDesc);
		allDeviceDescs.push_back((Format(u"GPU:%1% ,GPU Memory:%2%, System Memory:%3%, Shared Memory:%4%") % adapterDesc.Description % adapterDesc.DedicatedVideoMemory % adapterDesc.DedicatedSystemMemory % adapterDesc.SharedSystemMemory).str());
	}
	return allDeviceDescs;
}

bool GfxDeviceManager::CreateDevice(unsigned int index)
{
	if (index < 0 || index >= m_adapters.size())
		return false;

	m_curAdapter = m_adapters[index];
	for (unsigned int i = 0; i < m_adapters.size(); ++i)
		m_adapters[i]->Release();
	m_adapters.clear();
	return true;
}

GfxDeviceManager* GfxDeviceManager::m_instance = nullptr;