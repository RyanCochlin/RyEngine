#pragma once

#include <Windows.h>

#include "Utils/Singleton.h"

namespace RyEngine
{
	class RyEngineCore : public Singleton<RyEngineCore>
	{
	public:
		void Initialize();
		void CoreMain();
	};
}