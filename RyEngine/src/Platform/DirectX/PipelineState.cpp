#include "pch.h"
#include "PipelineState.h"
#include "RootSignature.h"

namespace RE
{
	PipelineState::PipelineState() :
		_mRootSignature(nullptr),
		_mPipelineState(nullptr)
	{
	}
	
	GraphicsPipelineState::GraphicsPipelineState() : 
		PipelineState()
	{
		ZeroMemory(&_mPSODescriptor, sizeof(_mPSODescriptor));
		//TODO as these params are needed I will pull them out into their own getters. For now set everything all at once
		_mPSODescriptor.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		_mPSODescriptor.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		_mPSODescriptor.DepthStencilState.DepthEnable = FALSE;
		_mPSODescriptor.DepthStencilState.StencilEnable = FALSE;
		_mPSODescriptor.SampleMask = UINT_MAX;
		_mPSODescriptor.NumRenderTargets = 1;
		_mPSODescriptor.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_mPSODescriptor.SampleDesc.Count = 1;
		_mPSODescriptor.SampleDesc.Quality = 0;
		_mPSODescriptor.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	}

	void GraphicsPipelineState::SetInputLayout(UINT numElements, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs)
	{
		//TODO check for numElements == 0
		D3D12_INPUT_ELEMENT_DESC* newElements = (D3D12_INPUT_ELEMENT_DESC*)malloc(sizeof(D3D12_INPUT_ELEMENT_DESC) * numElements);
		memcpy(newElements, inputElementDescs, numElements * sizeof(D3D12_INPUT_ELEMENT_DESC));

		_mPSODescriptor.InputLayout = { newElements, numElements };
	}

	void GraphicsPipelineState::Finalize(ID3D12Device* device)
	{
		_mPSODescriptor.pRootSignature = _mRootSignature->GetRootSignature().Get();

		ThrowIfFailed(device->CreateGraphicsPipelineState(&_mPSODescriptor, IID_PPV_ARGS(&_mPipelineState)));
	}
}
