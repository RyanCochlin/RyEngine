#include "pch.h"
#include "ViewPort.h"

namespace RE
{
	ViewPort::ViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
	{
		//_commandList = commandList;
		_viewPort.TopLeftX = topLeftX;
		_viewPort.TopLeftY = topLeftY;
		_viewPort.Width = width;
		_viewPort.Height = height;
		_viewPort.MinDepth = minDepth;
		_viewPort.MaxDepth = maxDepth;
		SetScissorRect( { 0, 0, (LONG)width, (LONG)height } );

		_aspectRatio = width / height;
	}

	void ViewPort::SetScissorRect(RECT rect)
	{
		_scissorRect = rect;
	}

	void ViewPort::Draw()
	{
		/*_commandList->RSSetViewports(1, &_viewPort);
		_commandList->RSSetScissorRects(1, &_scissorRect);*/
	}
}