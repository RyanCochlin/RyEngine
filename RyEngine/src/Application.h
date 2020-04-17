#pragma once

#include "Core/Core.h"

namespace RE
{
	class RE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Start();
		void Run();
	};

	Application* GetApplication();
}

