#pragma once

namespace RyEngine
{
	class IGraphicsAPI
	{
	public:
		virtual void Init() = 0;
		virtual void Release() = 0;
	};
}

