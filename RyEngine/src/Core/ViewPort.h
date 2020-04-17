#pragma once

namespace RE
{
	class ViewPort
	{
	public:
		ViewPort(ID3D12GraphicsCommandList* commandList, FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height, FLOAT minDepth = 0.0f, FLOAT maxDepth = 1.0f);
		void SetScissorRect(RECT rect);
		void Draw();

		D3D12_VIEWPORT viewPort() const { return _viewPort; }
		FLOAT AspectRatio() const { return _aspectRatio; }

	private:
		D3D12_VIEWPORT _viewPort{};
		RECT _scissorRect;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _commandList;

		FLOAT _aspectRatio;
	};
}

