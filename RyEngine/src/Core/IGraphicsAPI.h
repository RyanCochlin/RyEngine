#pragma once

namespace RE
{
	class IGraphicsAPI
	{
	public:
		virtual void Init() = 0;
		virtual void Release() = 0;
		virtual void OnRender() = 0;
	};
}

