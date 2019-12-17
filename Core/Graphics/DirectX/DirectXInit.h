//TODO select dxgi factory version from settings

#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <DXGI1_6.h>
#include <d3dcompiler.h>

#include "External/d3dx12.h"
#include "ViewPort.h"
#include "Graphics/IGraphicsAPI.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

namespace RyEngine
{
	class DirectXInit : public IGraphicsAPI
	{
	public:
		static const UINT BUFFER_COUNT = 2;

		void Init() override;
		void Release() override;

	protected:
		Microsoft::WRL::ComPtr<IDXGIFactory6> _dxgiFactory;
		Microsoft::WRL::ComPtr<ID3D12Device> _d3dDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _dsvHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> _renderTargets[BUFFER_COUNT];
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> _commandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _commandAllocator;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _bundleAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _commandList;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _bundleCommandList;
		Microsoft::WRL::ComPtr<ID3D12Resource> _depthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState;

	private:
		bool _useWarpDevice;
		bool _use4xMsaa;
		UINT _qualityLevels;
		UINT _currentBackBuffer;
		UINT _rtvDescriptorSize;
		UINT _dsvDescriptorSize;
		ViewPort* _mainView;

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
		void CreateViewPort();

		void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView();
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView();
	};
}