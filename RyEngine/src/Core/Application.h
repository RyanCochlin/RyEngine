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

		//TODO I don't think this is the right way to do this, but I need an update loop in the application prior to creating an ECS
		virtual void OnStart() {}
		virtual void OnUpdate() {}
	};

	Application* GetApplication();
}

