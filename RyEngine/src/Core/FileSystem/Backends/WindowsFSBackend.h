#pragma once

#include "Core/FileSystem/FileSystem.h"
#include "Core/FileSystem/FileBuffer.h"

#include <map>
#include <fstream>

namespace RE
{

class WindowsFSBackend : public IRFSBackend
{
public:
	virtual void Init(FileSystem* fs, RFSDrive* root) override;
	virtual void Deinit() override;
	virtual RFSFileHandle Open(const char* path, RFSPathHash pathHash) override;
	virtual size_t Read(RFSFileHandle file, RFSFileBuffer& fileBuffer) override;
	virtual const char* GetConfigPath() override;

private:
	RFSDrive* _mRoot;
	const char* _mConfigPath;
	uint32_t _mFileTotal;
	std::map<RFSFileHandle, std::fstream*> _mfileMap;
};

}