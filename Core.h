#pragma once

#include "Singleton.h"
#include "SubSystemManager.h"

namespace RyEngine
{
	class RyEngineCore : public Singleton<RyEngineCore>
	{
	public:
		void Initialize();
		void Run();
		void Release();
	};
}