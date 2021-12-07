//TODO select dxgi factory version from settings

#pragma once

#include "ViewPort.h"
#include "Core/Graphics/IGraphicsAPI.h"

using Microsoft::WRL::ComPtr;

namespace RE
{
	class DirectXInit
	{
	public:
		static const UINT BUFFER_COUNT = 2;

		void Init();
		void Release();

		ComPtr<ID3D12Resource> GetRenderTarget(UINT target);
		ID3D12Resource* GetCurrentBackBuffer();
		void UpdateCurrentBuffer();
		void UpdateCurrentFence();

		ComPtr<ID3D12Device> Device() const { return _d3dDevice; }
		ComPtr<ID3D12CommandQueue> CommandQueue() const { return _commandQueue; }
		ComPtr<ID3D12GraphicsCommandList> CommandList() const { return _commandList; }
		ComPtr<ID3D12CommandAllocator> CommandAllocator() const { return _commandAllocator; }
		ComPtr<ID3D12PipelineState> PipelineState() const { return _pipelineState; }
		ComPtr<ID3D12RootSignature> RootSignature() const { return _rootSignature; }
		ComPtr<ID3D12DescriptorHeap> RTVHeap() const { return _rtvHeap; }
		ComPtr<IDXGISwapChain> SwapChain() const { return _swapChain; }
		ComPtr<ID3D12Fence> Fence() const { return _fence; }

		//Test
		D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const { return _vertexBufferView; }

		UINT RTVDescSize() const { return _rtvDescriptorSize; }
		UINT BufferWidth() const { return _dxBufferWidth; }
		UINT BufferHeight() const { return _dxBufferHeight; }
		UINT CurrentFrame() const { return _currentBackBuffer; }
		HANDLE FenceEvent() const { return _fenceEvent; }
		UINT64 FenceValue() const { return _fenceValue; }

	protected:
		ComPtr<IDXGIFactory6> _dxgiFactory;
		ComPtr<ID3D12Device> _d3dDevice;
		ComPtr<IDXGISwapChain> _swapChain;
		ComPtr<ID3D12DescriptorHeap> _rtvHeap;
		ComPtr<ID3D12DescriptorHeap> _dsvHeap;
		ComPtr<ID3D12Resource> _renderTargets[BUFFER_COUNT];
		ComPtr<ID3D12CommandQueue> _commandQueue;
		ComPtr<ID3D12CommandAllocator> _commandAllocator;
		ComPtr<ID3D12CommandAllocator> _bundleAllocator;
		ComPtr<ID3D12GraphicsCommandList> _commandList;
		ComPtr<ID3D12GraphicsCommandList> _bundleCommandList;
		ComPtr<ID3D12Resource> _depthStencilBuffer;
		ComPtr<ID3D12RootSignature> _rootSignature;
		ComPtr<ID3D12PipelineState> _pipelineState;

		ComPtr<ID3D12Fence> _fence;
		HANDLE _fenceEvent;
		UINT64 _fenceValue;

	private:
		bool _useWarpDevice;
		bool _use4xMsaa;
		UINT _qualityLevels;
		UINT _currentBackBuffer;
		UINT _rtvDescriptorSize;
		UINT _dsvDescriptorSize;

		DXGI_FORMAT _dxSwapChainModeFormat;
		DXGI_MODE_SCANLINE_ORDER _dxSwapChainScanlineOrder;
		DXGI_MODE_SCALING _dxSwapChainModeScaling;
		DXGI_SWAP_EFFECT _dxSwapEffect;
		DXGI_SWAP_CHAIN_FLAG _dxSwapChainFlags;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC _psoDesc;
		UINT _dxBufferUsage;
		UINT _dxBufferWidth;
		UINT _dxBufferHeight;
		UINT _dxRefreshRate;
		UINT _dxMultiSampleCount;

		//Initialization
		void InitSettings();
		void CheckFeatureSupport();
		void LoadPipeline();
		void CreateCommandObjects();
		void EnableDebug();
		void CreateFactory();
		void CreateDevice();
		void CreateCommandQueue();
		void CreateCommandAllocators();
		void CreateCommandList();
		void CreateSwapChain();
		void CreateDescriptorHeap();
		void CreateRenderTarget();
		void CreateDepthStencilBuffer();
		void CreateRootSignature();
		void CreatePipelineState();
		void CreateFence();

		void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView();
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView();

		//Test
		ComPtr<ID3D12Resource> _vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
		void LoadTriangleTest();
	};
}