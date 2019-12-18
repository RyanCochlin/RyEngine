#include "ViewPort.h"

namespace RyEngine
{
	ViewPort::ViewPort(ID3D12GraphicsCommandList* commandList, FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height, FLOAT minDepth, FLOAT maxDepth)
	{
		_commandList = commandList;
		_viewPort.TopLeftX = topLeftX;
		_viewPort.TopLeftY = topLeftY;
		_viewPort.Width = width;
		_viewPort.Height = height;
		_viewPort.MinDepth = minDepth;
		_viewPort.MaxDepth = maxDepth;
		SetScissorRect( { 0, 0, (LONG)width, (LONG)height } );

	}

	void ViewPort::SetScissorRect(RECT rect)
	{
		_scissorRect = rect;
	}

	void ViewPort::Draw()
	{
		_commandList->RSSetViewports(1, &_viewPort);
		_commandList->RSSetScissorRects(1, &_scissorRect);
	}
}