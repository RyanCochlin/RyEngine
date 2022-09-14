#include "pch.h"

#include <string>

#include "DirectXCore.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "CommandManager.h"
#include "CommandContext.h"
#include "ColorBuffer.h"
#include "UploadBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
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
	ComPtr<ID3D12Device> DirectXCore::sDevice;

	DirectXCore::DirectXCore() :
		_mDisplayWidth(1920),
		_mDisplayHeight(1080),
		_mCurrentBuffer(0),
		_mSwapChainFormat(DXGI_FORMAT_R8G8B8A8_UNORM),
		_mCommandListManager(nullptr),
		_mCurrentCommandContext(nullptr),
		_mCurrentPassUploadResource(),
		_mCurrentObjectUploadResource(),
		_mCurrentMaterialUploadResource(),
		_mainView{},
		_mGeoManager{},
		_mRootSig{},
		_mSwapChain{}
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

			_mCommandListManager = new CommandListManager(sDevice.Get());
			_mCurrentCommandContext = new CommandContext(D3D12_COMMAND_LIST_TYPE_DIRECT, _mCommandListManager);
			_mCurrentCommandContext->Initialize();

			CreateSwapChain(factory.Get());

			for (uint16_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
			{
				//TODO release descriptor. Might be a memory leak
				ID3D12Resource* display;
				ThrowIfFailed(_mSwapChain->GetBuffer(i, IID_PPV_ARGS(&display)));
				D3D12_CPU_DESCRIPTOR_HANDLE handle = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				_mDisplays[i].CreateFromSwapChain(display, handle);
			}

			//Create Constant buffer.
			// TODO I think this should be done via draw call but I think it's okay here too
			_mCurrentPassUploadResource.Create(1);

			//Initialize PSO
			//TODO will probably want to do this on demand somehow but this will work for now
			_mRootSig = new RootSignature(sDevice.Get());
			_mPSO.SetRootSignature(_mRootSig);
			_mPSO.SetInputLayout(2, gILColoredDesc);
			_mPSO.SetPixelShader(g_scoloredPS, sizeof(g_scoloredPS));
			_mPSO.SetVertexShader(g_scoloredVS, sizeof(g_scoloredVS));
			_mPSO.SetTopology(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
			_mPSO.Finalize(sDevice.Get());
		}
		catch (DXException e)
		{
			RE_CORE_ERROR(e.ToString());
		}
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DirectXCore::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		return sDescriptorAllocators[type].Allocate(1, flags);
	}

	ComPtr<ID3D12DescriptorHeap> DirectXCore::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type)
	{
		return sDescriptorAllocators[type].GetCurrentDescriptorHeap();
	}

	uint32_t DirectXCore::GetDescriptorCount(D3D12_DESCRIPTOR_HEAP_TYPE type)
	{
		return sDescriptorAllocators[type].GetTotalAllocations();
	}

	UINT DirectXCore::GetDescriptorIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type)
	{
		return sDescriptorAllocators[type].GetDescriptorIncrementSize();
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
		ThrowIfFailed(factory->CreateSwapChain(comQueue, &swapChainDesc, _mSwapChain.GetAddressOf()));
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
		// TODO most of this should be based on inforation from a draw call but I haven't implemented those yet
		if (_mCurrentCommandContext == nullptr)
			return;

		// TODO some of these calls should be moved. Need to figure that out later
		_mCurrentCommandContext->Start();
		_mCurrentCommandContext->SetRootSignature(_mRootSig);
		_mCurrentCommandContext->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_mCurrentCommandContext->SetPipelineState(_mPSO);
		_mCurrentCommandContext->UploadMeshes(sDevice.Get(), _mGeoManager);
		_mCurrentCommandContext->End();
	}

	void DirectXCore::UploadPassConstantBuffers()
	{
		//TODO figure out how to make multiple draw calls. For now just use first one
		DXDrawCall dc = _mDrawCalls.back();
		ResColoredPassConstants pc{ dc.GetMVP(), dc.GetAmbient()};
		_mCurrentPassUploadResource.Upload(pc, 0);
		_mDrawCalls.clear();
	}

	void DirectXCore::UploadObjectConstantBuffers()
	{
		for (int i = 0; i < _mGeoManager.MeshCount(); ++i)
		{
			MeshGeometry& mesh = _mGeoManager.GetMesh(i);
			for (int j = 0; j < mesh.SubMeshCount(); ++j)
			{
				SubMeshData& subMesh = mesh.GetSubMeshData(j);
				ResColoredObjectConstants oc{ subMesh.transform->GetWorld() };
				ResColoredMaterialConstants mc;
				mc.DiffuseAlbedo = subMesh.material->materialData.diffuse;
				mc.FresnelR0 = subMesh.material->materialData.fresnel;
				mc.Roughness = subMesh.material->materialData.rough;

				_mCurrentObjectUploadResource.Upload(oc, j);
				_mCurrentMaterialUploadResource.Upload(mc, j);
			}
		}
	}

	void DirectXCore::SubmitMeshHeap(const MeshHeapData& meshHeap)
	{
		std::vector<Vertex> verts = meshHeap.vertexHeap;
		std::vector<RE_INDEX> indicies = meshHeap.indexHeap;
		VertexBuffer vb(verts);
		IndexBuffer ib(indicies);

		MeshGeometry mesh(vb, ib, meshHeap.subMeshData);
		uint32_t meshCount = meshHeap.subMeshData.size();
		_mGeoManager.AddMesh(mesh);
		_mCurrentObjectUploadResource.Create(meshCount);
		_mCurrentMaterialUploadResource.Create(meshCount);
	}

	void DirectXCore::DrawGeometery()
	{
		//This should just be for setting buffers
		for (int i = 0; i < _mGeoManager.MeshCount(); ++i)
		{
			MeshGeometry& mesh = _mGeoManager.GetMesh(i);

			for (int j = 0; j < mesh.SubMeshCount(); ++j)
			{
				_mCurrentCommandContext->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				_mCurrentCommandContext->SetVertexBuffers(mesh, 0);
				_mCurrentCommandContext->SetIndexBuffers(mesh);
				// TODO need to keep better track of the offsets and indicies for constat buffers
				uint32_t offset = j + 1;
				uint32_t matOffset = j + 3;
				_mCurrentObjectUploadResource.SetDescriptor(_mCurrentCommandContext, 1, offset);
				_mCurrentMaterialUploadResource.SetDescriptor(_mCurrentCommandContext, 2, matOffset);
				_mCurrentCommandContext->Draw(_mGeoManager.IndexCount(), _mGeoManager.VertexCount());
			}
		}
	}

	void DirectXCore::Update()
	{
		UploadPassConstantBuffers();
		UploadObjectConstantBuffers();

		if (_mGeoManager.IsDirty())
		{
			UploadGeometery();
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

		_mCurrentCommandContext->SetPipelineState(_mPSO);
		_mCurrentCommandContext->SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		_mCurrentPassUploadResource.SetDescriptor(_mCurrentCommandContext, 0, 0);
		//_mCurrentCommandContext->SetDescriptorTable(0, GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->GetGPUDescriptorHandleForHeapStart());
		
		_mCurrentCommandContext->TransitionResource(curBackBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
		_mCurrentCommandContext->SetRenderTarget(curBackBuffer.GetDescriptorHandle());
		_mCurrentCommandContext->SetViewPortAndScissor(0, 0, _mDisplayWidth, _mDisplayHeight);
		_mCurrentCommandContext->Clear(&curBackBuffer);

		DrawGeometery();

		_mCurrentCommandContext->TransitionResource(curBackBuffer, D3D12_RESOURCE_STATE_PRESENT);

		_mCurrentCommandContext->End();

		ThrowIfFailed(_mSwapChain->Present(1, 0));

		_mCurrentBuffer = (_mCurrentBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;
	}
}  