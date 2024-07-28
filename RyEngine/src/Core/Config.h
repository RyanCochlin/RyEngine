#pragma once

#include "pch.h"
#include <map>

namespace RE
{
	class Config
	{
	public:
		void SetVal(const char* config)
		{
			std::string confVal(config);
			auto pos = confVal.find(delim);
			ASSERT(pos != std::string::npos);

			std::string key = confVal.substr(0, pos);
			std::string val = confVal.substr(pos + 1, confVal.size());
			_mConfigVals.insert(std::pair<std::string,std::string>(key, val));
		}

		const char* GetVal(const char* key) const
		{
			auto it = _mConfigVals.find(std::string(key));
			ASSERT(it != _mConfigVals.end());
			return it->second.c_str();
		}

	private:
		const char delim = '|';
		std::map<std::string, std::string> _mConfigVals;
	};
}
