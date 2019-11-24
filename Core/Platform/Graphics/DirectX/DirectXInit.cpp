//TODO enumerate devices
//TODO check for warp device

#include "Assertions/Assert.h"
#include "Error/Error.h"
#include "DirectXInit.h"
#include "Platform/Window/Window.h"
#include "SubSystems/SubSystemManager.h"

using Microsoft::WRL::ComPtr;

namespace RyEngine
{
#pragma region Init

	void DirectXInit::Init()
	{
		_use4xMsaa = true;
		_currentBackBuffer = 0;
		LoadPipeline();
	}

	void DirectXInit::InitSettings()
	{
		_dxSwapChainModeFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		_dxSwapChainScanlineOrder = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		_dxSwapChainModeScaling = DXGI_MODE_SCALING_UNSPECIFIED;
		_dxBufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_dxSwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		_dxSwapChainFlags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		_dxBufferWidth = 800;
		_dxBufferHeight = 600;
		_dxRefreshRate = 60;
		_dxMultiSampleCount = 4;
	}

	void DirectXInit::LoadPipeline()
	{
		EnableDebug();
		CreateFactory();
		CreateDevice();
		CheckFeatureSupport();
		CreateCommandObjects();
		CreateSwapChain();
		CreateDescriptorHeap();
		CreateRenderTarget();
		CreateDepthStencilBuffer();
	}

	void DirectXInit::CheckFeatureSupport()
	{
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels;
		qualityLevels.Format = _dxSwapChainModeFormat;
		qualityLevels.SampleCount = _dxMultiSampleCount;
		qualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		qualityLevels.NumQualityLevels = 0;

		ThrowIfFailed(_d3dDevice->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&qualityLevels,
			sizeof(qualityLevels)
		));

		_qualityLevels = qualityLevels.NumQualityLevels;
		//TODO: should probably do something more than assert but it's good enough for now
		ASSERT(_qualityLevels > 0 && "Unexpected MSAA quality levels");
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
		Window* w = SubSystemManager::Instance().windowSystem().get_mainWindow();
		DXGI_SWAP_CHAIN_DESC sd;

		sd.BufferDesc.Width = _dxBufferWidth;
		sd.BufferDesc.Height = _dxBufferHeight;
		sd.BufferDesc.RefreshRate.Numerator = _dxRefreshRate;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = _dxSwapChainModeFormat;
		sd.BufferDesc.ScanlineOrdering = _dxSwapChainScanlineOrder;
		sd.BufferDesc.Scaling = _dxSwapChainModeScaling;
		sd.SampleDesc.Count = _use4xMsaa ? _dxMultiSampleCount : 1;
		sd.SampleDesc.Quality = _use4xMsaa ? (_qualityLevels - 1) : 0;
		sd.BufferUsage = _dxBufferUsage;
		sd.BufferCount = BUFFER_COUNT;
		sd.SwapEffect = _dxSwapEffect;
		sd.Flags = _dxSwapChainFlags;
		sd.Windowed = true; //TODO: This should be in the window manager subsystem
		sd.OutputWindow = w->get_hWND();

		ThrowIfFailed(_dxgiFactory->CreateSwapChain(
			_commandQueue.Get(),
			&sd,
			_swapChain.GetAddressOf()
		));
	}

	void DirectXInit::CreateDescriptorHeap()
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;

		rtvHeapDesc.NumDescriptors = BUFFER_COUNT;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;

		rtvHeapDesc.NumDescriptors = 1;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;

		ThrowIfFailed(_d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(_rtvHeap.GetAddressOf())));
		ThrowIfFailed(_d3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(_dsvHeap.GetAddressOf())));

		_rtvDescriptorSize = _d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		_dsvDescriptorSize = _d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	void DirectXInit::CreateRenderTarget()
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < BUFFER_COUNT; i++)
		{
			ThrowIfFailed(_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i])));
			_d3dDevice->CreateRenderTargetView(_renderTargets[i].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, _rtvDescriptorSize);
		}
	}

	void DirectXInit::CreateDepthStencilBuffer()
	{
		
	}
	
#pragma endregion

	D3D12_CPU_DESCRIPTOR_HANDLE DirectXInit::CurrentBackBufferView()
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
			_currentBackBuffer,
			_rtvDescriptorSize
		);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DirectXInit::DepthStencilView()
	{
		return _dsvHeap->GetCPUDescriptorHandleForHeapStart();
	}

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