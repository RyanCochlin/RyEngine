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

		RE_DECLARE_SUBSYSTEM(CameraSystem)

		void AddCamera(Camera* cam);
		void RemoveCamera(Camera* cam);
		std::vector<Camera*> GetCameras() const;

		void OnRegister() override;
		void OnStart() override;
		void OnUpdate() override;
		void OnRender() override;
		void Release() override;


	private:
		std::vector<Camera*> _mCameras;
	};
}
