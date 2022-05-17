#pragma once

#include <vector>
#include "Core/DirectXInit.h"
#include "Core/Color.h"
#include "Core/Graphics/MeshHeap.h"
#include "Core/Graphics/DrawCall.h"
#include "Core/Graphics/MeshManager.h"
#include "CommandManager.h"
#include "CommandContext.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "DescriptorAllocator.h"
#include "ColorBuffer.h"
#include "MeshGeometry.h"
#include "Resources/UploadResource.h"
#include "Resources/ResColored.h"

using Microsoft::WRL::ComPtr;

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

		void PushDrawCall(DrawCall d);
		void SetClearColor(Color color);
		
		static ComPtr<ID3D12Device> GetDevice() { return sDevice; }

		static D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
		static ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type);
		static uint32_t GetDescriptorCount(D3D12_DESCRIPTOR_HEAP_TYPE type);
		static UINT GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type);
		
	private:
		ComPtr<IDXGISwapChain> _mSwapChain;
		CommandListManager* _mCommandListManager;
		CommandContext* _mCurrentCommandContext;
		GraphicsPipelineState _mPSO;
		RootSignature* _mRootSig;
		ViewPort* _mainView;
		GeometeryManager _mGeoManager;
		std::vector<DrawCall> _mDrawCalls;
		DXGI_FORMAT _mSwapChainFormat;
		UINT32 _mDisplayWidth;
		UINT32 _mDisplayHeight;
		uint16_t _mCurrentBuffer;
		// TODO these need to be moved to the draw call
		UploadResource<ResColoredPassConstants> _mCurrentPassUploadResource;
		UploadResource<ResColoredObjectConstants> _mCurrentObjectUploadResource;

		static ComPtr<ID3D12Device> sDevice;
		static DescriptorAllocator sDescriptorAllocators[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

		ColorBuffer _mDisplays[SWAP_CHAIN_BUFFER_COUNT] = {
			ColorBuffer(DEFAULT_CLEAR_COLOR),
			ColorBuffer(DEFAULT_CLEAR_COLOR)
		};


		void CreateSwapChain(IDXGIFactory4* factory);
		void UploadGeometery();
		void UploadPassConstantBuffers();
		void UploadObjectConstantBuffers();
		void SubmitMeshHeap(const MeshHeapData& meshHeap);
		void DrawGeometery();
	};
}
