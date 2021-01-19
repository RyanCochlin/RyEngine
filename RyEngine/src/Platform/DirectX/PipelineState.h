#pragma once

#include "pch.h"
#include "RootSignature.h"

namespace RE
{
	class PipelineState
	{
	public:
		PipelineState();

		void SetRootSignature(const RootSignature* rootSig)
		{
			_mRootSignature = rootSig;
		}

		const RootSignature* GetRootSignature()
		{
			return _mRootSignature;
		}

		ID3D12PipelineState* GetPipelineStateObject() const
		{
			return _mPipelineState;
		}

	protected:
		const RootSignature* _mRootSignature;
		ID3D12PipelineState* _mPipelineState;
	};

	class GraphicsPipelineState : public PipelineState
	{
	public:
		GraphicsPipelineState();

		void Finalize(ID3D12Device* device);

		void SetInputLayout(UINT numElements, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs);
		void SetVertexShader(const void* buffer, size_t size) { _mPSODescriptor.VS = CD3DX12_SHADER_BYTECODE(buffer, size); }
		void SetPixelShader(const void* buffer, size_t size) { _mPSODescriptor.PS = CD3DX12_SHADER_BYTECODE(buffer, size); }
		void SetTopology(D3D12_PRIMITIVE_TOPOLOGY_TYPE type) { _mPSODescriptor.PrimitiveTopologyType = type; }

	private:
		D3D12_GRAPHICS_PIPELINE_STATE_DESC _mPSODescriptor;
	};
}