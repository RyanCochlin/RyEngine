#pragma once
#include "Camera.h"

#include <vector>

namespace RE
{
	class CameraManager : public SubSystem
	{
	public:
		CameraManager();
		~CameraManager();

		void AddCamera(Camera* cam);
		void RemoveCamera(Camera* cam);
		std::vector<Camera*> GetCameras() const;

		RE_SUBSYSTEM_OVERRIDE

	private:
		std::vector<Camera*> _mCameras;
	};
}
