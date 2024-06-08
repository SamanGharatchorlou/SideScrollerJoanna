#pragma once

#include "SRAC/System/Files/Config.h"
#include "SRAC/Animations/CharacterStates.h"
#include "SRAC/Animations/Animation.h"

struct AnimationConfig : public Config
{
	AnimationConfig(const char* config_name) : Config(config_name) { }
	void Read(XMLParser& parser) override;

	std::vector<Animation> animations;
	StringBuffer32 id;
};