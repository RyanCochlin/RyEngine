//TODO enumerate devices
//TODO check for warp device

#include "DirectXInit.h"

using Microsoft::WRL::ComPtr;

namespace RyGraphics
{
#pragma region Init

	void DirectXInit::Init()
	{
		LoadPipeline();
	}

	void DirectXInit::LoadPipeline()
	{
		EnableDebug();
		CreateFactory();
		CreateDevice();
		CreateCommandObjects();
	}
	
	// DirectX specific Implementation
	void DirectXInit::EnableDebug() 
	{
#if defined(DEBUG)
		ComPtr<ID3D12Debug> debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
#endif
	}

	void DirectXInit::CreateFactory()
	{
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory)));
	}

	void DirectXInit::CreateDevice()
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(&hardwareAdapter);

		ThrowIfFailed(D3D12CreateDevice(
			hardwareAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&_d3dDevice)
		));
	}

	void DirectXInit::CreateCommandObjects()
	{
		CreateCommandQueue();
		CreateCommandAllocators();
		CreateCommandList();
	}

	void DirectXInit::CreateCommandQueue()
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		ThrowIfFailed(_d3dDevice->CreateCommandQueue(
			&queueDesc,
			IID_PPV_ARGS(&_commandQueue)
		));
	}

	void DirectXInit::CreateCommandAllocators()
	{
		ThrowIfFailed(_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator)));
		ThrowIfFailed(_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&_bundleAllocator)));
	}

	void DirectXInit::CreateCommandList()
	{
		ThrowIfFailed(_d3dDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			_commandAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(&_commandList)
		)); 

		_commandList->Close();

		ThrowIfFailed(_d3dDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_BUNDLE,
			_bundleAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(&_bundleCommandList)
		));

		_bundleCommandList->Close();  
	}

	void DirectXInit::CreateSwapChain()
	{

	}

	void DirectXInit::CreateDescriptorHeap()
	{

	}

	void DirectXInit::CreateRenderTarget()
	{

	}
	
#pragma endregion

	void DirectXInit::GetHardwareAdapter(IDXGIAdapter1** ppAdapter)
	{
		*ppAdapter = nullptr;
		for (UINT aIndex = 0;; ++aIndex)
		{
			IDXGIAdapter1* pAdapter = nullptr;
			if (DXGI_ERROR_NOT_FOUND == _dxgiFactory->EnumAdapters1(aIndex, &pAdapter))
			{
				//Last adapter
				break;
			}

			if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
			{
				*ppAdapter = pAdapter;
				return;
			}

			pAdapter->Release();
		}
	}
}