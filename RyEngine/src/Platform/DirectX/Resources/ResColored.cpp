#include "pch.h"
#include "ResColored.h"
#include "Core/Math/CoreMath.h"
#include "Core/Math/Matrix.h"

namespace RE
{
	ResColouredUploadResource::ResColouredUploadResource() : 
		UploadResource()
	{
		_mDataSize = sizeof(ResColoredConstants);
	}

	ResColouredUploadResource::~ResColouredUploadResource()
	{
		_mUploadBuffer->Unmap();
		delete _mUploadBuffer;
	}

	void ResColouredUploadResource::Create()
	{
		_mSize = Math::Align(sizeof(Matrix4), 256);
		_mCount = 1;
		UploadResource::Create();
		_mCpuBuffer = _mUploadBuffer->Map();
	}

	void ResColouredUploadResource::Upload(Matrix4 vp)
	{
		ResColoredConstants c{ vp };
		memcpy(_mCpuBuffer, &c, _mDataSize);
	}
}