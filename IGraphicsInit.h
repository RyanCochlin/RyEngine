#pragma once

namespace RyEngine
{
	class IGraphicsInit
	{
		public:
			virtual ~IGraphicsInit() {}

			virtual void Init() = 0;
		private:
	};
}
