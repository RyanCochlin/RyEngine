#pragma once

namespace RE
{
	class IGraphicsInit
	{
		public:
			virtual ~IGraphicsInit() {}

			virtual void Init() = 0;
		private:
	};
}
