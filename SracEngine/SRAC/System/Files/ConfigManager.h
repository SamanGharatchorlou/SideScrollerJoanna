#pragma once

#include "Config.h"

class ConfigManager
{
public:
	static ConfigManager* Get();

	template<class T> 
	void add(const char* path)
	{
		if (!mConfigs.contains(path))
		{
			T* new_config = new T(path);
			mConfigs[path] = new_config;
		}
	}

	template<class T>
	T* addAndLoad(const char* path)
	{
		if (!mConfigs.contains(path))
		{
			T* new_config = new T(path);
			mConfigs[path] = new_config;
		}

		load();

		return static_cast<T*>(mConfigs[path]);
	}
	
	void load();

	template<class T>
	T* getConfig(const char* config)
	{
		if (mConfigs.contains(config))
		{
			ASSERT(mConfigs[config]->parsed, "config %s has not been parsed yet, no data");
			return static_cast<T*>(mConfigs[config]);
		}

		DebugPrint(Warning, "No config in the config manager with name: %s", config);
		return nullptr;
	}

private:
	//std::unordered_map<StringBuffer32, Config*> mConfigs;
	Map<StringBuffer32, Config*> mConfigs;
};