//TODO enumerate devices

#include "pch.h"
#include "d3d12.h"
#include "dxgi.h"
#include "SubSystemManager.h"
#include "SubSystem.h"
#include "DirectXInit.h"
#include "Vertex.h"
#include "Assert.h"
#include "Error.h"

using Microsoft::WRL::ComPtr;

namespace RE
{
#pragma region Init

	void DirectXInit::Init()
	{
		_use4xMsaa = false;
		_currentBackBuffer = 0;
		InitSettings();
		//EnableDebug();
		LoadPipeline();

		LoadTriangleTest();
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
		CreateFactory();
		CreateDevice();
		CheckFeatureSupport();
		CreateCommandObjects();
		CreateSwapChain();
		CreateDescriptorHeap();
		CreateRenderTarget();
		CreateCommandAllocators();
		CreateDepthStencilBuffer();
		CreateRootSignature();
		CreatePipelineState();
		CreateFence();
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
		ComPtr<IDXGIAdapter1> hardwareAdapter;
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
		Window* w = SubSystemManager::Instance().GetSubSystem<WindowSystem>()->GetMainWindow();
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
		//sd.OutputWindow = w->get_hWND();
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Note: Swap chain uses queue to perform flush.
		HRESULT res =_dxgiFactory->CreateSwapChain(
			_commandQueue.Get(),
			&sd,
			_swapChain.GetAddressOf());

		ThrowIfFailed(res);
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

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.Texture2D.MipSlice = 0;
		_d3dDevice->CreateDepthStencilView(
			_depthStencilBuffer.Get(),
			&dsvDesc,
			DepthStencilView()
		);
	}

	void DirectXInit::CreateRootSignature()
	{
		CD3DX12_ROOT_PARAMETER slotRootParameter[1];

		CD3DX12_DESCRIPTOR_RANGE cbvTable;
		cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		/*CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
		rootSigDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);*/

		ComPtr<ID3DBlob> serializedRootSig = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &errorBlob);

		if (errorBlob != nullptr)
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		ThrowIfFailed(hr);

		ThrowIfFailed(_d3dDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&_rootSignature)));
	}

	void DirectXInit::CreatePipelineState()
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

#if DEBUG
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		
		//TODO: need to figure out how to use a relative path to the shaders
		LPCWSTR shaderPath = L"C:\\Users\\rubby\\source\\repos\\RyEngine\\Shaders\\Shader.hlsl";
		LPCSTR vertexEntryPoint = "VSMain";
		LPCSTR vertexTarget = "vs_5_0";
		LPCSTR pixelEntryPoint = "PSMain";
		LPCSTR pixelTarget = "ps_5_0";

		ThrowIfFailed(D3DCompileFromFile(shaderPath, nullptr, nullptr, vertexEntryPoint, vertexTarget, compileFlags, 0, &vertexShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(shaderPath, nullptr, nullptr, pixelEntryPoint, pixelTarget, compileFlags, 0, &pixelShader, nullptr));

		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = _rootSignature.Get();
		psoDesc.VS = { reinterpret_cast<BYTE*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
		psoDesc.PS = { reinterpret_cast<BYTE*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = _use4xMsaa ? 4 : 1;
		psoDesc.SampleDesc.Quality = _use4xMsaa ? (_qualityLevels - 1) : 0;
		psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		ThrowIfFailed(_d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&_pipelineState)));
	}

	void DirectXInit::CreateFence()
	{
		ThrowIfFailed(_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
		_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}
	
#pragma endregion

	ComPtr<ID3D12Resource> DirectXInit::GetRenderTarget(UINT target)
	{
		return _renderTargets[target];
	}

	ID3D12Resource* DirectXInit::GetCurrentBackBuffer()
	{
		return _renderTargets[_currentBackBuffer].Get();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DirectXInit::CurrentBackBufferView()
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
			_currentBackBuffer,
			_rtvDescriptorSize
		);
	}

	void DirectXInit::UpdateCurrentBuffer()
	{
		_currentBackBuffer = (_currentBackBuffer + 1) % BUFFER_COUNT;
	}

	void DirectXInit::UpdateCurrentFence()
	{
		_fenceValue++;
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

#pragma region Test

	void DirectXInit::LoadTriangleTest()
	{
		FLOAT aspect = _dxBufferWidth / _dxBufferHeight;

		Vertex triangleVerts[] =
		{
			{ { 0.0f, 0.25f * aspect, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.25f, -0.25f * aspect, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.25f, -0.25f * aspect, 0.0f }, {1.0f, 0.0f, 1.0f, 1.0f } }
		};

		const UINT vertBufferSize = sizeof(triangleVerts);

		ThrowIfFailed(_d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(vertBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_vertexBuffer)
		));

		UINT8* pVertexDataBegin;
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
		memcpy(pVertexDataBegin, triangleVerts, sizeof(triangleVerts));
		_vertexBuffer->Unmap(0, nullptr);

		_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
		_vertexBufferView.StrideInBytes = sizeof(Vertex);
		_vertexBufferView.SizeInBytes = vertBufferSize;
	}

#pragma endregion
}