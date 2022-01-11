#include "pch.h"
#include "CommandContext.h"
#include "CommandManager.h"
#include "DirectXCore.h"
#include "VertexBuffer.h"
#include "MeshGeometry.h"
#include "ColorBuffer.h"

namespace RE
{
	CommandContext::CommandContext(D3D12_COMMAND_LIST_TYPE type, CommandListManager* commandManager) :
		_mCommandList(nullptr),
		_mCurrentAllocator(nullptr),
		_mCommandManager(commandManager),
		_mType(type)
	{}

	void CommandContext::Start()
	{
		//TODO allocate different types
		if (_mCurrentAllocator == nullptr)
		{
			_mCurrentAllocator = _mCommandManager->GetCommandQueue()->GetAllocator();
		}
		_mCommandList->Reset(_mCurrentAllocator, nullptr);
	}

	void CommandContext::SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type)
	{
		ComPtr<ID3D12DescriptorHeap> heap = DirectXCore::GetDescriptorHeap(type);
		if (heap != nullptr)
		{
			ID3D12DescriptorHeap* heaps[] = { heap.Get()};
			SetDescriptorHeaps(1, heaps);
		}
	}

	void CommandContext::SetDescriptorHeaps(UINT count, ID3D12DescriptorHeap* heaps[])
	{
		if (count > 0)
		{
			_mCommandList->SetDescriptorHeaps(count, heaps);
		}
	}

	void CommandContext::SetDescriptorTable(UINT index, D3D12_GPU_DESCRIPTOR_HANDLE handle)
	{
		_mCommandList->SetGraphicsRootDescriptorTable(index, handle);
	}

	void CommandContext::SetRootSignature(RootSignature* rootSig)
	{
		_mCommandList->SetGraphicsRootSignature(rootSig->GetRootSignature().Get());
	}

	void CommandContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
	{
		_mCommandList->IASetPrimitiveTopology(topology);
	}

	void CommandContext::SetPipelineState(PipelineState& pso)
	{
		ID3D12PipelineState* pipeline = pso.GetPipelineStateObject();
		if (_mPipelineState != pipeline)
		{
			_mPipelineState = pipeline;
		}
		_mCommandList->SetPipelineState(_mPipelineState);
	}

	void CommandContext::TransitionResource(GpuResource& resource, D3D12_RESOURCE_STATES newState)
	{
		D3D12_RESOURCE_STATES oldState = resource.GetCurrentState();
		_mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource.GetResource(), oldState, newState));
		resource.SetCurrentState(newState);
	}

	void CommandContext::SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtv)
	{
		SetRenderTargets(1, &rtv);
	}

	void CommandContext::SetRenderTargets(UINT numRTVs, D3D12_CPU_DESCRIPTOR_HANDLE rtvs[])
	{
		_mCommandList->OMSetRenderTargets(numRTVs, rtvs, false, nullptr);
	}

	void CommandContext::SetViewPortAndScissor(UINT x, UINT y, UINT w, UINT h)
	{
		SetViewPort(x, y, w, h);
		SetScissor(x, y, x + w, y + h);
	}

	void CommandContext::SetViewPort(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT minDepth, FLOAT maxDepth)
	{
		D3D12_VIEWPORT viewPort;
		viewPort.Width = w;
		viewPort.Height = h;
		viewPort.TopLeftX = x;
		viewPort.TopLeftY = y;
		viewPort.MinDepth = minDepth;
		viewPort.MaxDepth = maxDepth;
		_mCommandList->RSSetViewports(1, &viewPort);
	}

	void CommandContext::SetScissor(UINT left, UINT right, UINT top, UINT bottom)
	{
		SetScissor(CD3DX12_RECT(left, right, top, bottom));
	}

	void CommandContext::SetScissor(D3D12_RECT rect)
	{
		_mCommandList->RSSetScissorRects(1, &rect);
	}

	void CommandContext::SetVertexBuffers(GeometeryManager& gm, UINT slot)
	{
		if (gm.MeshCount() > 0)
		{
			//TODO need to fix this count for new Mesh type
			size_t count = gm.MeshCount();
			UINT validCount = 0;
			std::vector<D3D12_VERTEX_BUFFER_VIEW> vbView;

			for (int i = 0; i < count; i++)
			{
				MeshGeometry& mg = gm.GetMesh(i);
				if (mg.VertexCount() > 0)
				{
					D3D12_VERTEX_BUFFER_VIEW vbv = mg.VertexBufferView();
					vbView.push_back(vbv);
					validCount++;
				}
			}

			_mCommandList->IASetVertexBuffers(0, validCount, vbView.data());
		}
	}

	void CommandContext::SetVertexBuffers(MeshGeometry& mesh, UINT slot)
	{
		std::vector<D3D12_VERTEX_BUFFER_VIEW> vbView;
		if (mesh.VertexCount() > 0)
		{
			D3D12_VERTEX_BUFFER_VIEW vbv = mesh.VertexBufferView();
			vbView.push_back(vbv);
			_mCommandList->IASetVertexBuffers(0, 1, vbView.data());
		}
	}

	void CommandContext::SetIndexBuffers(GeometeryManager& gm)
	{
		if (gm.MeshCount() > 0)
		{
			size_t count = gm.MeshCount();
			std::vector<D3D12_INDEX_BUFFER_VIEW> ibView;

			for (int i = 0; i < count; i++)
			{
				MeshGeometry& mg = gm.GetMesh(i);
				if (mg.IndexCount() > 0)
				{
					D3D12_INDEX_BUFFER_VIEW vbv = mg.IndexBufferView();
					ibView.push_back(vbv);
				}
			}

			_mCommandList->IASetIndexBuffer(ibView.data());
		}
	}

	void CommandContext::SetIndexBuffers(MeshGeometry & mesh)
	{
		std::vector<D3D12_INDEX_BUFFER_VIEW> ibView;
		if (mesh.IndexCount() > 0)
		{
			D3D12_INDEX_BUFFER_VIEW ibv = mesh.IndexBufferView();
			ibView.push_back(ibv);
		}

		_mCommandList->IASetIndexBuffer(ibView.data());
	}

	void CommandContext::Draw(ColorBuffer* buffer, UINT indexCount, UINT vertexCount)
	{
		Color clearColor = buffer->GetClearColor();;
		D3D12_CPU_DESCRIPTOR_HANDLE rtv = buffer->GetDescriptorHandle();

		_mCommandList->ClearRenderTargetView(rtv, clearColor.rgba, 0, nullptr);
		//_mCommandList->DrawInstanced(vertexCount, 1, 0, 0);
		_mCommandList->DrawIndexedInstanced(indexCount, vertexCount, 0, 0, 0);
	}

	void CommandContext::UploadMeshes(ID3D12Device* device, GeometeryManager& gm)
	{
		gm.UploadAll(_mCommandList);
	}

	void CommandContext::UploadConstantBuffers(ID3D12Device* device)
	{

	}

	void CommandContext::End()
	{
		CommandQueue* queue = _mCommandManager->GetCommandQueue();
		uint64_t fence = queue->Execute(_mCommandList);
		_mCommandManager->WaitForFence(fence);
		queue->ReleaseAllocator(_mCurrentAllocator);
		_mCurrentAllocator = nullptr;
	}

	void CommandContext::Initialize()
	{
		_mCommandManager->CreateCommandList(_mType, &_mCommandList, &_mCurrentAllocator);
		_mCommandList->Close();
	}

}