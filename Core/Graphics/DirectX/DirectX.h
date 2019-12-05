#pragma once
#include "Graphics/IGraphicsAPI.h"
#include "DirectXInit.h"

namespace RyEngine
{
	class DirectX : public IGraphicsAPI
	{
	public:
		void Init() override;
		void Release() override;
		
	private:
		DirectXInit* _init;
	};
}

