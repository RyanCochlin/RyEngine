#include "pch.h"
#include "CameraManager.h"

#include <vector>

namespace RE
{
	CameraManager::CameraManager() :
		_mCameras()
	{
	}

	CameraManager::~CameraManager()
	{
		for (std::vector<Camera*>::iterator i = _mCameras.begin(); i != _mCameras.end(); i++)
		{
			delete *i;
		}

		_mCameras.clear();
	}

	void CameraManager::AddCamera(Camera* cam)
	{
		//TODO make sure this camera isn't already in the list
		_mCameras.push_back(cam);
	}

	void CameraManager::RemoveCamera(Camera* cam)
	{
		for (std::vector<Camera*>::iterator i = _mCameras.begin(); i != _mCameras.end(); i++)
		{
			if (*i == cam)
			{
				_mCameras.erase(i);
				break;
			}
		}
	}

	std::vector<Camera*> CameraManager::GetCameras() const
	{
		return _mCameras;
	}

	void CameraManager::OnRegister() {}
	void CameraManager::OnRender() {}
	void CameraManager::OnStart() {}
	void CameraManager::Release() {}

	void CameraManager::OnUpdate()
	{
		for (std::vector<Camera*>::iterator i = _mCameras.begin(); i != _mCameras.end(); i++)
		{
			(*i)->Update();
		}
	}


}