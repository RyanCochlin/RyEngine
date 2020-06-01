#pragma once

#include "Core/IGraphicsAPI.h"
#include "Core/DirectXInit.h"
#include "Core/Color.h"
#include "CommandManager.h"
#include "DescriptorAllocator.h"
#include "ColorBuffer.h"

namespace RE
{

#define SWAP_CHAIN_BUFFER_COUNT 2
#define SWAP_CHAIN_REFRESH_RATE 60
#define DEFAULT_CLEAR_COLOR Color{0.0f, 1.0f, 1.0f, 1.0f}

	class RyDirectX : public IGraphicsAPI
	{
		friend class DirectxAPI;
		friend class ColorBuffer;
	public:
		RyDirectX();
		~RyDirectX();

		void Init() override;
		void Release() override;
		void OnRender() override;

		D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type);
		
	private:
		ComPtr<ID3D12Device> _mDevice;
		ComPtr<IDXGISwapChain> _mSwapChain;		
		CommandListManager* _mCommandListManager;
		ViewPort* _mainView;
		DXGI_FORMAT _mSwapChainFormat;
		UINT32 _mDisplayWidth;
		UINT32 _mDisplayHeight;
		UINT32 _mCurrentBuffer;

		ColorBuffer _mDisplays[SWAP_CHAIN_BUFFER_COUNT] = {
			ColorBuffer(DEFAULT_CLEAR_COLOR),
			ColorBuffer(DEFAULT_CLEAR_COLOR)
		};

		DescriptorAllocator _mDescriptorAllocators[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] = {
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		};

		void CreateSwapChain(IDXGIFactory6* factory);
		void PopulateCommandList();
		void WaitForPreviousFrame();

		//TODO: this is just test stuff to draw a triangle
		void CreateTriangle();
	};
}
