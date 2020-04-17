#pragma once

#include "pch.h"

namespace RE
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
