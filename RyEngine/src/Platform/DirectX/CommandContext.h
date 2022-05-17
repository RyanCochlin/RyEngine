#pragma once
#include "CommandManager.h"
#include "PipelineState.h"
#include "GpuResource.h"
#include "VertexBuffer.h"
#include "MeshGeometry.h"
#include "ColorBuffer.h"

namespace RE
{
	class CommandContext
	{
	public:
		CommandContext(D3D12_COMMAND_LIST_TYPE type, CommandListManager* commandManager);

		void Initialize();
		void Start();
		void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type);
		void SetDescriptorHeaps(UINT count, ID3D12DescriptorHeap* heaps[]);
		void SetDescriptorTable(UINT index, D3D12_GPU_DESCRIPTOR_HANDLE handle);
		void SetRootSignature(RootSignature* rootSig);
		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology);
		void SetPipelineState(PipelineState& pso);
		void TransitionResource(GpuResource& resource, D3D12_RESOURCE_STATES newState);
		void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtv);
		void SetRenderTargets(UINT numRTVs, D3D12_CPU_DESCRIPTOR_HANDLE rtvs[]);
		void SetViewPortAndScissor(UINT x, UINT y, UINT w, UINT h);
		void SetViewPort(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT minDepth = 0.0f, FLOAT maxDepth = 1.0f);
		void SetScissor(UINT left, UINT right, UINT top, UINT bottom);
		void SetScissor(D3D12_RECT rect);
		void SetVertexBuffers(GeometeryManager& gm, UINT slot);
		void SetVertexBuffers(MeshGeometry& mesh, UINT slot);
		void SetIndexBuffers(GeometeryManager& gm);
		void SetIndexBuffers(MeshGeometry& mesh);
		void SetDescriptorHeaps();
		void Clear(ColorBuffer* buffer);
		void Draw(UINT indexCount, UINT vertexCount);
		void UploadMeshes(ID3D12Device* device, GeometeryManager& gm);
		void End();

	private:
		CommandListManager* _mCommandManager;
		ID3D12GraphicsCommandList* _mCommandList;
		ID3D12CommandAllocator* _mCurrentAllocator;
		ID3D12PipelineState* _mPipelineState;
		D3D12_COMMAND_LIST_TYPE _mType;
	};
}