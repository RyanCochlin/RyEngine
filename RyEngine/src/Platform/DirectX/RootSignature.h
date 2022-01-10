#pragma once

namespace RE
{
	using Microsoft::WRL::ComPtr;
	class RootSignature
	{
	public:
		//TODO this will need to be expanded on for more root parameters
		RootSignature(ID3D12Device* device);
		~RootSignature() = default;

		ComPtr<ID3D12RootSignature> GetRootSignature() const { return _mRootSig; }

	private:
		ComPtr<ID3D12RootSignature> _mRootSig;
	};
}