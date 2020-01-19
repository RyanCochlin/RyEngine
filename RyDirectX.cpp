#include "stdafx.h"
#include "RyDirectX.h"
#include "Color.h"
#include "CoreMath.h"

namespace RyEngine
{
	void RyDirectX::Init()
	{
		_init = new DirectXInit();
		_init->Init();
		_mainView = new ViewPort(_init->CommandList().Get(), 0.0f, 0.0f, static_cast<FLOAT>(_init->BufferWidth()), static_cast<FLOAT>(_init->BufferHeight()));
	}

	void RyDirectX::Release()
	{
		_init->Release();
		delete _mainView;
		delete _init;
	}

	void RyDirectX::OnRender()
	{
		PopulateCommandList();

		ID3D12CommandList* ppCommandLists[] = { _init->CommandList().Get() };
		_init->CommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		ThrowIfFailed(_init->SwapChain()->Present(0, 0));

		WaitForPreviousFrame();
	}

	void RyDirectX::PopulateCommandList()
	{
		ThrowIfFailed(_init->CommandAllocator()->Reset());

		ThrowIfFailed(_init->CommandList()->Reset(_init->CommandAllocator().Get(), _init->PipelineState().Get()));
		
		_init->CommandList()->SetGraphicsRootSignature(_init->RootSignature().Get());
		_mainView->Draw();

		_init->CommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(_init->GetCurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_init->RTVHeap()->GetCPUDescriptorHandleForHeapStart(), _init->CurrentFrame(), _init->RTVDescSize());
		_init->CommandList()->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		Color clearColor = { 0.0f, 1.0f, 1.0f, 1.0f };
		_init->CommandList()->ClearRenderTargetView(rtvHandle, clearColor.rbga, 0, nullptr);

		_init->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_init->CommandList()->IASetVertexBuffers(0, 1, &(_init->VertexBufferView()));
		_init->CommandList()->DrawInstanced(3, 1, 0, 0);

		_init->CommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(_init->GetCurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		ThrowIfFailed(_init->CommandList()->Close());
	}

	void RyDirectX::WaitForPreviousFrame()
	{
		const UINT64 fence = _init->FenceValue();
		ThrowIfFailed(_init->CommandQueue()->Signal(_init->Fence().Get(), fence));
		_init->UpdateCurrentFence();

		if (_init->Fence()->GetCompletedValue() < fence)
		{
			ThrowIfFailed(_init->Fence()->SetEventOnCompletion(fence, _init->FenceEvent()));
			WaitForSingleObject(_init->FenceEvent(), INFINITE);
		}

		_init->UpdateCurrentBuffer();
	}

	//TODO: test stuff
	void RyDirectX::CreateTriangle()
	{
		//Vertex verts[] =
		//{
		//	{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
		//};
	}
}  