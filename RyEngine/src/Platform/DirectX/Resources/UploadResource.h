#pragma once

#include "Platform/DirectX/UploadBuffer.h"

namespace RE
{
	class UploadResource
	{
	public:
		UploadResource();
		virtual ~UploadResource();

		virtual void Create();

		void GetCpuHandle();
		void GetGpuHandle();

	protected:
		UploadBuffer* _mUploadBuffer;
		D3D12_CPU_DESCRIPTOR_HANDLE _mCBV;
		UINT _mSize;
		UINT _mCount;
	};
}