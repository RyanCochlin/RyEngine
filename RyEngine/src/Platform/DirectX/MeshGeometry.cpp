#include "pch.h"
#include "MeshGeometry.h"
#include "VertexBuffer.h"
#include "UploadBuffer.h"
#include "IndexBuffer.h"
#include "CommandManager.h"
#include "Core/Mesh.h"
#include "Core/Vertex.h"
#include "Core/SubSystemManager.h"

namespace RE
{
	GeometeryManager::GeometeryManager():
		_mMeshes()
	{}

	GeometeryManager::~GeometeryManager()
	{
		std::vector<MeshGeometry*>::iterator i = _mMeshes.begin();
		for (; i != _mMeshes.end(); i++)
		{
			delete *i;
		}
		_mMeshes.clear();
	}

	void GeometeryManager::Submit(Mesh mesh)
	{
		MeshGeometry* m = new MeshGeometry(mesh);
		_mMeshes.push_back(m);
	}

	void GeometeryManager::UploadAll(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
	{
		std::vector<MeshGeometry*>::iterator i = _mMeshes.begin();
		for (; i != _mMeshes.end(); i++)
		{
			MeshGeometry* geo = *i;
			if (geo->_mUploaded)
				continue;

			//TODO this stuff should be put into it's own method to remove code replication
			VertexBuffer* vertBuf = geo->_mGpuResource;
			UploadBuffer* upBuf = geo->_mVertexUploadResource;
			upBuf->Create(geo->VertexCount(), geo->ElementSize());
			vertBuf->Create(device, geo->VertexCount(), geo->ElementSize());

			D3D12_SUBRESOURCE_DATA data = geo->GetResourceData();
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(vertBuf->GetResource(), vertBuf->GetCurrentState(), D3D12_RESOURCE_STATE_COPY_DEST));
			vertBuf->SetCurrentState(D3D12_RESOURCE_STATE_COPY_DEST);
			UpdateSubresources<1>(commandList, vertBuf->GetResource(), upBuf->GetResource(), 0, 0, 1, &data);
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(vertBuf->GetResource(), vertBuf->GetCurrentState(), D3D12_RESOURCE_STATE_GENERIC_READ));
			vertBuf->SetCurrentState(D3D12_RESOURCE_STATE_GENERIC_READ);


			IndexBuffer* indBuf = geo->_mIndexBuffer;
			UploadBuffer* indUpBuf = geo->_mIndexUploadResource;
			indUpBuf->Create(geo->IndexCount(), geo->IndexElementSize());
			indBuf->Create(device, geo->IndexCount(), geo->IndexElementSize());

			D3D12_SUBRESOURCE_DATA indexData = geo->GetIndexResourceData();
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(indBuf->GetResource(), indBuf->GetCurrentState(), D3D12_RESOURCE_STATE_COPY_DEST));
			indBuf->SetCurrentState(D3D12_RESOURCE_STATE_COPY_DEST);
			UpdateSubresources<1>(commandList, indBuf->GetResource(), indUpBuf->GetResource(), 0, 0, 1, &indexData);
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(indBuf->GetResource(), indBuf->GetCurrentState(), D3D12_RESOURCE_STATE_GENERIC_READ));
			indBuf->SetCurrentState(D3D12_RESOURCE_STATE_GENERIC_READ);

			geo->_mUploaded = true;
		}
	}

	UINT GeometeryManager::MeshCount()
	{
		return _mMeshes.size();
	}

	UINT GeometeryManager::VertexCount()
	{
		UINT total = 0;
		std::vector<MeshGeometry*>::iterator i = _mMeshes.begin();
		for (; i != _mMeshes.end(); i++)
		{
			MeshGeometry* mesh = *i;
			total += mesh->VertexCount();
		}

		return total;
	}

	UINT GeometeryManager::IndexCount()
	{
		UINT total = 0;
		std::vector<MeshGeometry*>::iterator i = _mMeshes.begin();
		for (; i != _mMeshes.end(); i++)
		{
			MeshGeometry* mesh = *i;
			total += mesh->IndexCount();
		}

		return total;
	}

	MeshGeometry* GeometeryManager::GetMesh(int index)
	{
		ASSERT(index < MeshCount());

		return _mMeshes[index];
	}

	//----------------------------------Mesh Geometry------------------------------//
	MeshGeometry::MeshGeometry() :
		_mUploaded(false),
		_mVerticies{}
	{
		_mGpuResource = new VertexBuffer();
		_mIndexBuffer = new IndexBuffer();
		_mVertexUploadResource = new UploadBuffer();
		_mIndexUploadResource = new UploadBuffer();
	}

	MeshGeometry::MeshGeometry(Mesh m) :
		_mUploaded(false),
		_mVerticies{}
	{
		FLOAT aspect = SubSystemManager::Instance().GetSubSystem<WindowSystem>()->GetMainWindowAspect();
		_mGpuResource = new VertexBuffer();
		_mVertexUploadResource = new UploadBuffer();
		_mIndexUploadResource = new UploadBuffer();
		_mIndexBuffer = new IndexBuffer();

		std::vector<Vertex> verts = m.GetVerticies();
		Vector4 c = ColorToVector(m.GetColor());
		for (auto i = verts.begin(); i != verts.end(); i++)
		{
			/*Vector3 pos = *i;
			Vertex v{ pos, c };*/
			_mVerticies.push_back( *i );
		}

		std::vector<RE_INDEX> inds = m.GetIndicies();
		for (auto i = inds.begin(); i != inds.end(); i++)
		{
			_mIndicies.push_back(*i);
		}
	}

	MeshGeometry::~MeshGeometry()
	{
		_mVerticies.clear();
		_mIndicies.clear();
		delete _mGpuResource;
		delete _mIndexBuffer;
		delete _mVertexUploadResource;
		delete _mIndexUploadResource;
	}

	Vertex& MeshGeometry::operator[](int index)
	{
		return _mVerticies[index];
	}

	Vertex* MeshGeometry::GetData()
	{
		return _mVerticies.data();
	}

	D3D12_VERTEX_BUFFER_VIEW MeshGeometry::VertexBufferView()
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = _mGpuResource->GetGpuAddress();
		vbv.StrideInBytes = sizeof(Vertex);
		vbv.SizeInBytes = _mVerticies.size() * sizeof(Vertex);
		return vbv;
	}

	D3D12_INDEX_BUFFER_VIEW MeshGeometry::IndexBufferView()
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = _mIndexBuffer->GetGpuAddress();
		ibv.Format = DXGI_FORMAT_R16_UINT;
		ibv.SizeInBytes = _mIndicies.size() * sizeof(RE_INDEX);
		return ibv;
	}

	D3D12_SUBRESOURCE_DATA MeshGeometry::GetResourceData()
	{
		Vertex* data = _mVerticies.data();
		D3D12_SUBRESOURCE_DATA resource;

		resource.pData = data;
		resource.RowPitch = VertexCount() * ElementSize();
		resource.SlicePitch = resource.RowPitch;

		return resource;
	}

	D3D12_SUBRESOURCE_DATA MeshGeometry::GetIndexResourceData()
	{
		RE_INDEX* data = _mIndicies.data();
		D3D12_SUBRESOURCE_DATA resource;

		resource.pData = data;
		resource.RowPitch = IndexCount() * IndexElementSize();
		resource.SlicePitch = resource.RowPitch;

		return resource;
	}
}