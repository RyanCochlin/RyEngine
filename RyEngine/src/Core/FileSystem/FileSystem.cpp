#include "pch.h"
#include "FileSystem.h"
#include "FileBuffer.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include "Core/Config.h"
#include "Backends/WindowsFSBackend.h"
#include "Core/SubSystemManager.h"
#include <unordered_map>

namespace RE
{
	void FileSystem::OnStart()
	{
#ifdef RE_WINDOWS
		// need to just use std methods here before 
		// TODO make a string builder to make this work
		std::string prjPath(PRJ_PATH);
		std::string cfgPath(CONFIG_PATH);
		std::string cfgFilePath = prjPath + cfgPath;
		std::fstream stream(cfgFilePath, std::ios::in);
		ASSERT(stream.is_open());
		Config cfg;
		std::string line;
		while(std::getline(stream, line))
		{
			cfg.SetVal(line.c_str());
		}
		
		const char* loc = cfg.GetVal("asset_folder");
		SetBackend(RFS_WINDOWS, "/assets/", loc);
#endif
	}

	void FileSystem::Release()
	{
		for (auto it : _mExistnigBackends)
		{
			auto backend = it.second;
			backend->Deinit();
		}
	}

	void FileSystem::SetBackend(RFS_BACKEND_TYPE backendType, std::string root, std::string physicalLocation)
	{
		IRFSBackend* backend;
		auto it = _mExistnigBackends.find(backendType);
		if (it == _mExistnigBackends.end())
		{
			backend = CreateBackend(backendType);
			_mExistnigBackends.insert(std::pair<RE::RFS_BACKEND_TYPE, IRFSBackend*>(backendType, backend));
		}
		else
		{
			backend = it->second;
		}
		
		RFSDriveHash driveHash = GetDriveHash(root.c_str());
		_mBackendMap.insert(std::pair<RFSDriveHash, IRFSBackend*>(driveHash, backend));
		
		backend->AddDrive(driveHash, physicalLocation);
		
		//TODO i I don't know if we need this anymore
		RFSDrive* drive = new RFSDrive();
		drive->drive = root;
		drive->physicalLocation = physicalLocation;
		backend->Init(this, drive);
	}

	RFSDriveHandle FileSystem::GetDriveHandle(const char* pathRoot)
	{
		//Get backend root handle and find it in the backend map
		std::string s(pathRoot);
		size_t pos = s.find('/');
		ASSERT(pos != std::string::npos);
		const char* drive = s.substr(0, pos).c_str();
		RFSDriveHash driveHash = GetDriveHash(drive);
		auto backendIt = _mBackendMap.find(driveHash);
		ASSERT(backendIt != _mBackendMap.end());
		
		//TODO open path in backend specified by path root
		
		IRFSBackend* backend = backendIt->second;
		RFSDriveHandle handle( backend );
		return handle;
	}

	RFSFileHandle FileSystem::Open(const char* drive, const char* path)
	{
		RFSDriveHash driveHash = GetDriveHash(drive);
		RFSPathHash pathHash = GetPathHash(path);
		auto it = _mBackendMap.find(driveHash);
		if (it == _mBackendMap.end())
		{
			RE_CORE_ERROR("File System drive not found: {0}", drive);
		}
		
		auto fileHandle = it->second->Open(path, pathHash);
		_mDriveHandleMap.insert(std::pair<RFSFileHandle, RFSDriveHash>(fileHandle, driveHash));
		return fileHandle;
	}

	void FileSystem::Close(RFSFileHandle handle)
	{
		// TODO fill this out
	}

	size_t FileSystem::Read(RFSFileHandle file, RFSFileBuffer& fileBuffer)
	{
		auto it = _mDriveHandleMap.find(file);
		if (it == _mDriveHandleMap.end())
		{
			RE_CORE_WARN("File not found");
		}

		auto it2 = _mBackendMap.find(it->second);
		if (it2 == _mBackendMap.end())
		{
			RE_CORE_WARN("Bakcend not found");
		}

		IRFSBackend* backend = it2->second;
		return backend->Read(file, fileBuffer);
	}

	RFSDriveHash FileSystem::GetDriveHash(const char* drive)
	{
		std::hash<std::string> hashMethod;
		uint32_t hash = static_cast<uint32_t>(hashMethod(std::string(drive)));
		return hash;
	}

	RFSPathHash FileSystem::GetPathHash(const char* path)
	{
		return GetDriveHash(path);
	}

	std::string FileSystem::GetDrivePath(const char* path)
	{
		// TODO fill this out
		return "";
	}

	IRFSBackend* FileSystem::CreateBackend(RFS_BACKEND_TYPE beType)
	{
		IRFSBackend* be = nullptr;
		switch (beType)
		{
		case RFS_WINDOWS:
			be = new WindowsFSBackend();
			break;
		default:
			RE_CORE_ERROR("Invalid FileSystem Type {0}", beType);
		}
		
		return be;
	}

	RFSDriveAndPath FileSystem::GetDriveAndPath(const char* fullPath)
	{
		const char delim = '/';
		std::string strPath(fullPath);
		auto it = strPath.find(delim);
		if (it == std::string::npos || it != 0)
		{
			RE_CORE_ERROR("Invalid filesystem path: {0}", strPath);
		}
		
		auto it2 = strPath.find(delim, it + 1);
		if (it2 == std::string::npos)
		{
			RE_CORE_ERROR("Invalid filesystem path: {0}", strPath);
		}

		std::string drive = strPath.substr(0, it2 + 1);
		std::string path = strPath.substr(it2 + 1);
		return { drive, path };
	}


	// API 
	RFSFileHandle FileSystem::FileOpen(const char* path)
	{
		RFSDriveAndPath driveAndPath = GetDriveAndPath(path);
		auto fileHandle = SubSystemManager::Instance().GetSubSystem<FileSystem>()->Open(std::get<0>(driveAndPath).c_str(), std::get<1>(driveAndPath).c_str());
		return fileHandle;
	}

	size_t FileSystem::FileRead(RFSFileHandle file, RFSFileBuffer& fileBuffer)
	{
		return SubSystemManager::Instance().GetSubSystem<FileSystem>()->Read(file, fileBuffer);
	}
}