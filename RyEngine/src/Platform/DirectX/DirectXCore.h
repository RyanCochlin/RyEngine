#pragma once

#include "Core/DirectXInit.h"
#include "Core/Color.h"
#include "Core/GeometryHeap.h"
#include "CommandManager.h"
#include "CommandContext.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "DescriptorAllocator.h"
#include "ColorBuffer.h"
#include "MeshGeometry.h"

namespace RE
{

#define SWAP_CHAIN_BUFFER_COUNT 2
#define SWAP_CHAIN_REFRESH_RATE 60
#define DEFAULT_CLEAR_COLOR Color{0.0f, 1.0f, 1.0f, 1.0f}

	class DirectXCore
	{
		friend class DirectxAPI;
		friend class ColorBuffer;

	public:
		DirectXCore();
		~DirectXCore();

		void Init();
		void Update();
		void Release();
		void OnRender();

		void SetClearColor(Color color);
		
		ID3D12Device* GetDevice() { return _mDevice; }

		D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type);
		
	private:
		ID3D12Device* _mDevice;
		IDXGISwapChain* _mSwapChain;		
		CommandListManager* _mCommandListManager;
		CommandContext* _mCurrentCommandContext;
		GraphicsPipelineState* _mPSO;
		RootSignature* _mRootSig;
		ViewPort* _mainView;
		GeometeryManager* _mGeoManager;
		DXGI_FORMAT _mSwapChainFormat;
		UINT32 _mDisplayWidth;
		UINT32 _mDisplayHeight;
		uint16_t _mCurrentBuffer;

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
		void UploadGeometery();
		void SubmitGeometery(GeometryHeap* geo);

		//TODO: this is just test stuff to draw a triangle
		void CreateTriangle();
	};
}
