#pragma once
#include "Graphics/IGraphicsAPI.h"
#include "SubSystems/SubSystemManager.h"
#include "DirectXInit.h"
#include "Error/Error.h"
#include "Graphics/Color.h"

namespace RyEngine
{
	class DirectX : public IGraphicsAPI
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
	};
}

