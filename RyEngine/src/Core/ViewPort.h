#pragma once

namespace RE
{
	class ViewPort
	{
	public:
		ViewPort() {}
		ViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);
		
		void SetScissorRect(RECT rect);
		void Draw();

		D3D12_VIEWPORT viewPort() const { return _viewPort; }
		float AspectRatio() const { return _aspectRatio; }

	private:
		D3D12_VIEWPORT _viewPort{};
		RECT _scissorRect;
		//Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _commandList;

		float _aspectRatio;
	};
}

class ViewPort
{
};

