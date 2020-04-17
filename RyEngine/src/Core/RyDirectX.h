#pragma once

#include "IGraphicsAPI.h"
#include "DirectXInit.h"

namespace RE
{
	class RyDirectX : public IGraphicsAPI
	{
	public:
		void Init() override;
		void Release() override;
		void OnRender() override;
		
	private:
		DirectXInit* _init;
		ViewPort* _mainView;

		void PopulateCommandList();
		void WaitForPreviousFrame();

		//TODO: this is just test stuff to draw a triangle
		void CreateTriangle();
	};
}
