#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <Windows.h>

namespace RyEngine
{
	template < class T > class Singleton
	{
	public:
		static T& Instance()
		{
			static T instance;
			return instance;
		}

	protected:
		Singleton() {}
		~Singleton() {}
	};
}
