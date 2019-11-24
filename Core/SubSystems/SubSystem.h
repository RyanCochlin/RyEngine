#pragma once

namespace RyEngine
{
	class SubSystem
	{
	public:
		virtual void OnRegister() = 0;
		virtual void OnStart() = 0;
	protected:
	};
}

