#include "pch.h"
#include "ConfigManager.h"

#include "Config.h"


ConfigManager* ConfigManager::Get()
{
	GameData& gd = GameData::Get();
	ASSERT(gd.configs != nullptr, "Config manager has no been set up yet");
	return gd.configs;
}

static void GetFullPath(const char* name, BasicString& out_path)
{
	out_path = FileManager::Get()->findFile(FileManager::Configs, name);
	if (out_path.empty())
	{
		DebugPrint(Warning, "No config file found named %s found in config folder", name);
	}
}

void ConfigManager::load()
{
	XMLParser* parser = new XMLParser;

	for (auto iter = mConfigs.begin(); iter != mConfigs.end(); iter++)
	{
		Config* config = iter->second;
		if (config->parsed)
			continue;

		BasicString full_path;
		GetFullPath(iter->first.c_str(), full_path);
		parser->parseXML(full_path.c_str());


		config->Read(*parser);
		config->parsed = true;
	}

	delete parser;
}

