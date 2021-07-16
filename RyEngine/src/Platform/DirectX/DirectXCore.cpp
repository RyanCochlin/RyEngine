#include "pch.h"

#include <string>

#include "DirectXCore.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "CommandManager.h"
#include "CommandContext.h"
#include "ColorBuffer.h"
#include "UploadBuffer.h"
#include "DXDrawCall.h"
#include "Core/Color.h"
#include "Core/Math/Vector.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Core/SubSystemManager.h"
#include "CompiledShaders/coloredPS.h"
#include "CompiledShaders/coloredVS.h"
#include "InputLayouts/ILColored.h"
#include "Resources/ResColored.h"
#include "ErrorHandling/DXException.h"

namespace RE
{
	DescriptorAllocator DirectXCore::sDescriptorAllocators[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] = {
		   D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		   D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
		   D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		   D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
	};
	ID3D12Device* DirectXCore::sDevice;

	DirectXCore::DirectXCore() :
		_mDisplayWidth(1920),
		_mDisplayHeight(1080),
		_mCurrentBuffer(0),
		_mSwapChainFormat(DXGI_FORMAT_R8G8B8A8_UNORM),
		_mCommandListManager(nullptr),
		_mCurrentCommandContext(nullptr),
		_mCurrentUploadResource(nullptr),
		_mainView{},
		_mGeoManager{},
		_mRootSig{},
		_mSwapChain{},
		_mUploaded(false)
	{}

	DirectXCore::~DirectXCore()
	{}

	void DirectXCore::Init()
	{
		//TODO check feature support
		try
		{
#if DEBUG
			//TODO This causes a memory leak! Potentially enable with a cusom flag.
			ComPtr<ID3D12Debug> debugController;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
			debugController->EnableDebugLayer();
#endif

			ComPtr<IDXGIFactory4> factory;
			ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

			ComPtr<IDXGIAdapter1> pAdapter;
			size_t max = 0;

			for (uint32_t i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &pAdapter); ++i)
			{
				DXGI_ADAPTER_DESC1 desc;
				ThrowIfFailed(pAdapter->GetDesc1(&desc));

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					continue;
				}

				if (desc.DedicatedVideoMemory > max && SUCCEEDED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&sDevice))))
				{
					pAdapter->GetDesc1(&desc);
					RE_CORE_LOG("D3D12 hardware found: {0} ({1} MB)", WCharPToString(desc.Description), (desc.DedicatedVideoMemory >> 20));
					max = desc.DedicatedVideoMemory;
				}
			}

			if (sDevice == nullptr)
			{
				RE_CORE_WARN("D3D12 hardware not found. Falling back to WARP");
				ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter)));
				ThrowIfFailed(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&sDevice)));
			}

			//Set width and height from window system
			WindowDimensions wd = SubSystemManager::Instance().GetSubSystem<WindowSystem>()->GetMainWindow()->GetDimensions();
			_mDisplayWidth = wd.width;
			_mDisplayHeight = wd.height;

			_mCommandListManager = new CommandListManager(sDevice);
			_mCurrentCommandContext = new CommandContext(D3D12_COMMAND_LIST_TYPE_DIRECT, _mCommandListManager);
			_mCurrentCommandContext->Initialize();

			CreateSwapChain(factory.Get());

			for (uint16_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
			{
				//TODO release descriptor. Might be a memory leak
				ID3D12Resource* display;
				ThrowIfFailed(_mSwapChain->GetBuffer(i, IID_PPV_ARGS(&display)));
				D3D12_CPU_DESCRIPTOR_HANDLE handle = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				_mDisplays[i].CreateFromSwapChain(sDevice, display, handle);
			}

			//Create Constant buffer.
			//TODO I think this should be done via draw call but I think it's okay here too
			_mCurrentUploadResource = new ResColouredUploadResource();
			_mCurrentUploadResource->Create();

			//Initialize PSO
			//TODO will probably want to do this on demand somehow but this will work for now
			_mRootSig = new RootSignature(sDevice);
			_mPSO.SetRootSignature(_mRootSig);
			_mPSO.SetInputLayout(2, gILColoredDesc);
			_mPSO.SetPixelShader(g_scoloredPS, sizeof(g_scoloredPS));
			_mPSO.SetVertexShader(g_scoloredVS, sizeof(g_scoloredVS));
			_mPSO.SetTopology(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
			_mPSO.Finalize(sDevice);

			//TODO get the shader working with identity matrix. That will show I'm uploading the data to the buffer correctly.
		}
		catch (DXException e)
		{
			RE_CORE_ERROR(e.ToString());
		}
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DirectXCore::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		return sDescriptorAllocators[type].Allocate(sDevice, 1, flags);
	}

	ID3D12DescriptorHeap* DirectXCore::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type)
	{
		return sDescriptorAllocators[type].GetCurrentDescriptorHeap();
	}

	void DirectXCore::CreateSwapChain(IDXGIFactory4* factory)
	{
		WindowsWindow* wnd = static_cast<WindowsWindow*>(SubSystemManager::Instance().GetSubSystem<WindowSystem>()->GetMainWindow());

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = _mDisplayWidth;
		swapChainDesc.BufferDesc.Height = _mDisplayHeight;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = SWAP_CHAIN_REFRESH_RATE;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = _mSwapChainFormat;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDesc.Windowed = true;
		swapChainDesc.OutputWindow = wnd->GetHandle();

		//TODO: Use CreateSwapChainForHwind and CreateSwapChainforCoreWindow to support UWP using IDXGISwapChain1
		ID3D12CommandQueue* comQueue = _mCommandListManager->GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT)->GetCommandQueue();
		ThrowIfFailed(factory->CreateSwapChain(comQueue, &swapChainDesc, &_mSwapChain));
	}

	void DirectXCore::Release()
	{
		_mCommandListManager->Shutdown();

		delete _mCommandListManager;
		delete _mCurrentCommandContext;
		delete _mainView;
		delete _mRootSig;
	}

	void DirectXCore::PushDrawCall(DrawCall d)
	{
		_mDrawCalls.push_back(d);
	}

	void DirectXCore::SetClearColor(Color color)
	{
		for (uint16_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		{
			_mDisplays[i].SetClearColor(color);
		}
	}

	void DirectXCore::UploadGeometery()
	{
		if (_mCurrentCommandContext == nullptr)
			return;

		_mCurrentCommandContext->Start();
		_mCurrentCommandContext->SetRootSignature(_mRootSig);
		_mCurrentCommandContext->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_mCurrentCommandContext->SetPipelineState(_mPSO);
		_mCurrentCommandContext->UploadMeshes(sDevice, _mGeoManager);
		_mCurrentCommandContext->End();
	}

	void DirectXCore::UploadConstantBuffers()
	{
		//TODO figure out how to make multiple draw calls. For now just use first one
		if (_mCurrentUploadResource != nullptr)
		{
			DXDrawCall dc = _mDrawCalls.back();
			dynamic_cast<ResColouredUploadResource*>(_mCurrentUploadResource)->Upload(dc.GetMVP()/*I4*/);
			_mDrawCalls.clear();
		}
	}

	void DirectXCore::SubmitGeometery(GeometryHeap* geo)
	{
		std::vector<Mesh*> meshes = geo->GetHeap();

		std::vector<Mesh*>::iterator i = meshes.begin();
		for (; i != meshes.end(); i++)
		{
			_mGeoManager.Submit(**i);
		}
	}

	void DirectXCore::Update()
	{
		UploadConstantBuffers();
		//TODO I need to figure out how to upload only when the geometry changes
		if (!_mUploaded)
		{
			UploadGeometery();
			_mUploaded = true;
		}
	}

	void DirectXCore::OnRender()
	{
		if (_mCurrentCommandContext == nullptr)
			return;

		//TODO update current back buffer
		ColorBuffer curBackBuffer = _mDisplays[_mCurrentBuffer];
		
		_mCurrentCommandContext->Start();
		_mCurrentCommandContext->SetRootSignature(_mRootSig);
		_mCurrentCommandContext->SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		_mCurrentCommandContext->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_mCurrentCommandContext->SetPipelineState(_mPSO);
		_mCurrentCommandContext->SetDescriptorTable(0, GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->GetGPUDescriptorHandleForHeapStart());
		_mCurrentCommandContext->TransitionResource(curBackBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
		_mCurrentCommandContext->SetRenderTarget(curBackBuffer.GetDescriptorHandle());
		_mCurrentCommandContext->SetViewPortAndScissor(0, 0, _mDisplayWidth, _mDisplayHeight);
		_mCurrentCommandContext->SetVertexBuffers(_mGeoManager, 0);
		_mCurrentCommandContext->SetIndexBuffers(_mGeoManager);
		_mCurrentCommandContext->Draw(&curBackBuffer, _mGeoManager.IndexCount(), _mGeoManager.VertexCount());
		_mCurrentCommandContext->TransitionResource(curBackBuffer, D3D12_RESOURCE_STATE_PRESENT);

		_mCurrentCommandContext->End();

		ThrowIfFailed(_mSwapChain->Present(1, 0));

		_mCurrentBuffer = (_mCurrentBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;

		//PopulateCommandList();

		//ID3D12CommandList* ppCommandLists[] = { _init->CommandList().Get() };
		//_init->CommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		/*ThrowIfFailed(_init->SwapChain()->Present(0, 0));

		WaitForPreviousFrame();*/
	}

	void DirectXCore::PopulateCommandList()
	{
		//ThrowIfFailed(_init->CommandAllocator()->Reset());

		/*ThrowIfFailed(_init->CommandList()->Reset(_init->CommandAllocator().Get(), _init->PipelineState().Get()));
		
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

		ThrowIfFailed(_init->CommandList()->Close());*/
	}

	void DirectXCore::WaitForPreviousFrame()
	{
		/*const UINT64 fence = _init->FenceValue();
		ThrowIfFailed(_init->CommandQueue()->Signal(_init->Fence().Get(), fence));
		_init->UpdateCurrentFence();

		if (_init->Fence()->GetCompletedValue() < fence)
		{
			ThrowIfFailed(_init->Fence()->SetEventOnCompletion(fence, _init->FenceEvent()));
			WaitForSingleObject(_init->FenceEvent(), INFINITE);
		}

		_init->UpdateCurrentBuffer();*/
	}

	//TODO: test stuff
	void DirectXCore::CreateTriangle()
	{
		//Vertex verts[] =
		//{
		//	{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
		//};
	}
}  