//TODO enumerate devices

#include "Assertions/Assert.h"
#include "Error/Error.h"
#include "DirectXInit.h"
#include "Window/Window.h"
#include "Graphics/Color.h"
#include "SubSystems/SubSystemManager.h"

using Microsoft::WRL::ComPtr;

namespace RyEngine
{
#pragma region Init

	void DirectXInit::Init()
	{
		_use4xMsaa = false;
		_currentBackBuffer = 0;
		InitSettings();
		LoadPipeline();
	}

	void DirectXInit::Release()
	{

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
		CreateViewPort(); //This is not ready yet
	}

	void DirectXInit::CheckFeatureSupport()
	{
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels;
		qualityLevels.Format = _dxSwapChainModeFormat;
		qualityLevels.SampleCount = _dxMultiSampleCount;
		qualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		qualityLevels.NumQualityLevels = 0;

		/*ThrowIfFailed(*/HRESULT hr = _d3dDevice->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&qualityLevels,
			sizeof(qualityLevels)
		);

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

		HRESULT result = D3D12CreateDevice(
			hardwareAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&_d3dDevice)
		);

		if (FAILED(result))
		{
			ComPtr<IDXGIAdapter> warpAdapter;
			ThrowIfFailed(_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

			ThrowIfFailed(D3D12CreateDevice(
				warpAdapter.Get(),
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&_d3dDevice)
			));
		}
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
		//DXGI_SWAP_CHAIN_DESC sd;
		//SecureZeroMemory(&sd, sizeof(sd));

		_swapChain.Reset();

		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = _dxBufferWidth;
		sd.BufferDesc.Height = _dxBufferHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = _dxSwapChainModeFormat;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = _use4xMsaa ? 4 : 1;
		sd.SampleDesc.Quality = _use4xMsaa ? (_qualityLevels - 1) : 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = BUFFER_COUNT;
		sd.OutputWindow = w->get_hWND();
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Note: Swap chain uses queue to perform flush.
		ThrowIfFailed(_dxgiFactory->CreateSwapChain(
			_commandQueue.Get(),
			&sd,
			_swapChain.GetAddressOf()));
	}

	void DirectXInit::CreateDescriptorHeap()
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;

		rtvHeapDesc.NumDescriptors = BUFFER_COUNT;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;

		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;

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
		D3D12_RESOURCE_DESC depthStencilDesc;
		depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthStencilDesc.Alignment = 0;
		depthStencilDesc.Width = _dxBufferWidth;
		depthStencilDesc.Height = _dxBufferHeight;
		depthStencilDesc.DepthOrArraySize = 1;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthStencilDesc.SampleDesc.Count = _use4xMsaa ? 4 : 1;
		depthStencilDesc.SampleDesc.Quality = _use4xMsaa ? _qualityLevels - 1 : 0;
		depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE optClear;
		optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		optClear.DepthStencil.Depth = 1.0f;
		optClear.DepthStencil.Stencil = 0;

		ThrowIfFailed(_d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,
			&optClear,
			IID_PPV_ARGS(&_depthStencilBuffer)
		));

		_d3dDevice->CreateDepthStencilView(
			_depthStencilBuffer.Get(),
			nullptr,
			DepthStencilView()
		);

		_commandList->ResourceBarrier(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				_depthStencilBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATE_DEPTH_WRITE
			)
		);
	}

	void DirectXInit::CreateViewPort()
	{
		//TODO: most of this should change I just want to get to a point where I'm clearing the screen with a color
		//ThrowIfFailed(_commandAllocator->Reset());

		//ThrowIfFailed(_commandList->Reset(_commandAllocator.Get(), nullptr));

		_mainView = new ViewPort(_commandList.Get(), 0.0f, 0.0f, static_cast<FLOAT>(_dxBufferWidth), static_cast<FLOAT>(_dxBufferHeight));

		//ID3D12Resource* currentBackBuffer = _renderTargets[_currentBackBuffer].Get();
		//_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer,
			//D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		// Clear the back buffer and depth buffer.
		//Color color = { 0.0f, 1.0f, 1.0f, 1.0f };
		//_commandList->ClearRenderTargetView(CurrentBackBufferView(), color.rbga, 0, nullptr);
		//_commandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
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