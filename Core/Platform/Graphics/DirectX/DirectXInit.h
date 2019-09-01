//TODO select dxgi factory version from settings

#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <DXGI1_6.h>

#include "../../../Core.cpp"
#include "../RyGraphics.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

namespace RyGraphics
{
	class DirectXInit : public IGraphicsInit
	{
	public:
		void Init();
	protected:
		Microsoft::WRL::ComPtr<IDXGIFactory6> _dxgiFactory;
		Microsoft::WRL::ComPtr<ID3D12Device> _d3dDevice;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> _commandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _commandAllocator;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _bundleAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _commandList;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _bundleCommandList;
	private:
		bool _useWarpDevice;

		//Initialization
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

		void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);
	};
}