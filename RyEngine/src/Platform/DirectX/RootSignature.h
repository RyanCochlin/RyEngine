#pragma once

namespace RE
{
	class RootSignature
	{
	public:
		//TODO this will need to be expanded on for more root parameters
		RootSignature(ID3D12Device* device);
		~RootSignature();

		ID3D12RootSignature* GetRootSignature() const { return _mRootSig; }

	private:
		ID3D12RootSignature* _mRootSig;
	};
}