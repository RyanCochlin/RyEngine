#pragma once

#include "pch.h"
#include "Core/SubSystem.h"
#include "FileBuffer.h"
#include <map>

// TODO file system needs A LOT of work!
namespace RE
{
#define CONFIG_PATH "config/fs.cfg"

	class FileSystem;

	using RFSDriveHash = uint32_t;
	using RFSPathHash = uint32_t;
	using RFSFileHandle = uint32_t;
	using RFSDriveAndPath = std::tuple<std::string, std::string>;

	enum RFS_BACKEND_TYPE
	{
		RFS_NONE = 0,
		RFS_WINDOWS = 1,
		RSF_COUNT = 2
	};

	struct RFSDrive
	{
		std::string drive;
		std::string physicalLocation;
	};

	class IRFSBackend
	{
	public:
		virtual void Init(FileSystem* fs, RFSDrive* root) = 0;
		virtual void Deinit() = 0;
		virtual const char* GetConfigPath() = 0;
		virtual RFSFileHandle Open(const char* path, RFSPathHash pathHash) = 0;
		virtual size_t Read(RFSFileHandle file, RFSFileBuffer& fileBuffer) = 0;
		virtual void AddDrive(RFSDriveHash root, std::string physicalLocation)
		{
			_mFileRootPaths.insert(std::pair<RFSDriveHash, std::string>(root, physicalLocation));
		}

	protected:
		FileSystem* _mFileSystem;
		std::map<RFSDriveHash, std::string> _mFileRootPaths;
		std::map<RFSFileHandle, std::fstream> _mFileHandleMap;
	};

	class RFSDriveHandle
	{
	public:
		RFSDriveHandle(IRFSBackend* backend) :
			_mBackend(backend)
		{}

	private:
		IRFSBackend* _mBackend;
	};

	class FileSystem : public SubSystem
	{
	public:
		RE_DECLARE_SUBSYSTEM(FileSystem)

		RE_API static RFSFileHandle FileOpen(const char* path);
		RE_API static size_t FileRead(RFSFileHandle file, RFSFileBuffer& fileBuffer);

		virtual void OnRegister() override {}
		virtual void OnStart() override;
		virtual void OnUpdate() override {}
		virtual void OnRender() override {}
		virtual void Release() override;

		void SetBackend(RFS_BACKEND_TYPE backend, std::string root, std::string physicalLocation);
		RFSDriveHandle GetDriveHandle(const char* pathRoot); // TODO dont think I need this
		RFSFileHandle Open(const char* drive, const char* path);
		void Close(RFSFileHandle handle);
		size_t Read(RFSFileHandle file, RFSFileBuffer& fileBuffer);

		static RFSDriveAndPath GetDriveAndPath(const char* path);

	private:
		RFSDriveHash GetDriveHash(const char* drive);
		RFSPathHash GetPathHash(const char* path);
		std::string GetDrivePath(const char* path);
		IRFSBackend* CreateBackend(RFS_BACKEND_TYPE beType);

	private:
		std::map<RFSDriveHash, IRFSBackend*> _mBackendMap;
		std::map<RFSDriveHandle, RFSDrive*> _mRootDriveMap;
		std::map<RFSFileHandle, RFSDriveHash> _mDriveHandleMap;
		std::map<RFS_BACKEND_TYPE, IRFSBackend*> _mExistnigBackends;
	};

	// TODO build File System external API
	
}