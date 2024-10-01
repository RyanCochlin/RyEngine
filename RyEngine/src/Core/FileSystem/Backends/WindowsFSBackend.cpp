#include "pch.h"

#include <iostream>
#include <fstream>

#include "WindowsFSBackend.h"
#include "Core/FileSystem/FileSystem.h"
#include "Core/FileSystem/FileBuffer.h"


namespace RE
{

void WindowsFSBackend::Init(FileSystem* fs, RFSDrive* root)
{
	_mFileSystem = fs;
	_mRoot = root;
	_mConfigPath = nullptr;
	_mFileTotal = 0;
}

void WindowsFSBackend::Deinit()
{
	for (auto streamIt : _mfileMap)
	{
		auto stream = streamIt.second;
		stream->close();
		delete stream;
	}
}

RFSFileHandle WindowsFSBackend::Open(const char* path, RFSPathHash pathHash)
{
	std::string sPath(path);
	std::string loc(_mRoot->physicalLocation); 
	std::string fullPath = loc + sPath;
	std::fstream *stream = new std::fstream(fullPath, std::ios::ate | std::ios::in | std::ios::out);
	ASSERT(stream->is_open());

	RFSFileHandle current = _mFileTotal;
	_mfileMap[current] = stream;
	_mFileTotal++;
	return current;
}

size_t WindowsFSBackend::Read(RFSFileHandle file, RFSFileBuffer& fileBuffer)
{
	auto it = _mfileMap.find(file);
	ASSERT(it != _mfileMap.end());
	std::fstream* stream = it->second;
	ASSERT(stream->is_open());

	stream->seekg(0, std::ios::end);
	std::streamsize size = stream->tellg();
	stream->seekg(0, std::ios::beg);

	char* buf = new char[size + 1];
	if (stream->read(buf, size))
	{
		fileBuffer.buffer = buf;
		fileBuffer.size = size;
		return size;
	}
	return 0;
}

const char* WindowsFSBackend::GetConfigPath()
{
	if (_mConfigPath == nullptr)
	{
		std::string prjPath(PRJ_PATH);
		std::string cfgPath(CONFIG_PATH);
		std::string fullPath = prjPath + cfgPath;
		_mConfigPath = fullPath.c_str();
	}
	return _mConfigPath;
}

} //RE