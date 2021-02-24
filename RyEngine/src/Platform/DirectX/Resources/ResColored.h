#pragma once
#include "Core/Math/Matrix.h"
#include "UploadResource.h"

namespace RE
{
	struct ResColoredConstants
	{
		Matrix4 ViewProj;
	};

	class ResColouredUploadResource : public UploadResource
	{
	public:
		ResColouredUploadResource();
		virtual ~ResColouredUploadResource();

		void Upload(Matrix4 vp);
		void Create() override;

	private:
		ResColoredConstants _mConstantBufferData;
		size_t _mDataSize;
		void* _mCpuBuffer;
	};
}