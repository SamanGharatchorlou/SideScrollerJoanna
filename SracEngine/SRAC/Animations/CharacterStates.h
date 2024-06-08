#pragma once

#include <unordered_map> // could remove this by moving it into .cpp
#include "Core/StringBuffers.h"

class Texture;

enum class ActionState
{
	None,

	Idle,
	Walk,
	Run,

	Attack,
	SlashAttack,

	Hurt,
	Dead,

	Count
};

static std::unordered_map<StringBuffer32, ActionState> s_actionMap;

static void initActionMap()
{
	s_actionMap.reserve((size_t)ActionState::Count);

	s_actionMap["None"] = ActionState::None;

	s_actionMap["Idle"] = ActionState::Idle;
	s_actionMap["Walk"] = ActionState::Walk;
	s_actionMap["Run"] = ActionState::Run;
	s_actionMap["Attack"] = ActionState::Attack;
	s_actionMap["SlashAttack"] = ActionState::SlashAttack;

	//s_actionMap["Jump"] = ActionState::Jump;
	//s_actionMap["Fall"] = ActionState::Fall;

	//s_actionMap["AirAttack"] = ActionState::AirAttack;
	//s_actionMap["LightAttack"] = ActionState::LightAttack;
	//s_actionMap["UpLightAttack"] = ActionState::UpLightAttack;
	//s_actionMap["HeavyAttack"] = ActionState::HeavyAttack;

	//s_actionMap["Alert"] = ActionState::Alert;
	//s_actionMap["Hurt"] = ActionState::Hurt;
	//s_actionMap["Dead"] = ActionState::Dead;
}

static ActionState stringToAction(const char* action)
{
	if (s_actionMap.empty())
		initActionMap();

	return s_actionMap.at(action);
}

static StringBuffer32 actionToString(ActionState action)
{
	if (s_actionMap.empty())
		initActionMap();

	for (auto iter = s_actionMap.begin(); iter != s_actionMap.end(); iter++)
	{
		if (iter->second == action)
			return iter->first;
	}

	return StringBuffer32();
}