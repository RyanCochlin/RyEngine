#include "pch.h"
#include "MeshGeometry.h"
#include "VertexBuffer.h"
#include "UploadBuffer.h"
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

			VertexBuffer* vertBuf = geo->_mGpuResource;
			UploadBuffer* upBuf = geo->_mUploadResource;
			upBuf->Create(geo->Count(), geo->ElementSize());
			vertBuf->Create(device, geo->Count(), geo->ElementSize());

			D3D12_SUBRESOURCE_DATA data = geo->GetResourceData();
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(vertBuf->GetResource(), vertBuf->GetCurrentState(), D3D12_RESOURCE_STATE_COPY_DEST));
			vertBuf->SetCurrentState(D3D12_RESOURCE_STATE_COPY_DEST);
			UpdateSubresources<1>(commandList, vertBuf->GetResource(), upBuf->GetResource(), 0, 0, 1, &data);
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(vertBuf->GetResource(), vertBuf->GetCurrentState(), D3D12_RESOURCE_STATE_GENERIC_READ));
			vertBuf->SetCurrentState(D3D12_RESOURCE_STATE_GENERIC_READ);

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
			total += mesh->Count();
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
		_mUploadResource = new UploadBuffer();
	}

	MeshGeometry::MeshGeometry(Mesh m) :
		_mUploaded(false),
		_mVerticies{}
	{
		FLOAT aspect = RE_WINDOW->GetMainWindowAspect();
		_mGpuResource = new VertexBuffer();
		_mUploadResource = new UploadBuffer();
		std::vector<Vector3> verts = m.GetVerticies();
		Vector4 c = ColorToVector(m.GetColor());

		for (auto i = verts.begin(); i != verts.end(); i++)
		{
			Vector3 pos = *i;
			pos.y *= aspect;
			Vertex v{ pos, c };
			_mVerticies.push_back( v );
		}
	}

	MeshGeometry::~MeshGeometry()
	{
		_mVerticies.clear();
		delete _mGpuResource;
		delete _mUploadResource;
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

	D3D12_SUBRESOURCE_DATA MeshGeometry::GetResourceData()
	{
		Vertex* data = _mVerticies.data();
		D3D12_SUBRESOURCE_DATA resource;

		resource.pData = data;
		resource.RowPitch = Count() * ElementSize();
		resource.SlicePitch = resource.RowPitch;

		return resource;
	}
}