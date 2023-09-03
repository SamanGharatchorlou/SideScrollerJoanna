#pragma once

#include <unordered_map>
#include "Core/StringBuffers.h"
#include "Core/Vector2D.h"

class Texture;

enum class ActionState
{
	None,

	Idle,
	Walk,
	Run,
	AirAttack,
	LightAttack,
	UpLightAttack,
	HeavyAttack,
	Alert,
	Hurt,
	Dead,

	TRANSITIONS,

	IdleToWalk,
	WalkToIdle,

	IdleToRun,
	WalkToRun,
	RunToIdle,

	Count
};


// move this to common / no gamelib stuff?
struct StateTransition
{
	ActionState from = ActionState::None;
	ActionState to = ActionState::None;
	ActionState transitionAction = ActionState::None;
};

static std::unordered_map<StringBuffer32, ActionState> s_actionMap;

static void initActionMap()
{
	s_actionMap.reserve((size_t)ActionState::Count);

	s_actionMap["None"] = ActionState::None;

	s_actionMap["Idle"] = ActionState::Idle;
	s_actionMap["Walk"] = ActionState::Walk;
	s_actionMap["Run"] = ActionState::Run;

	s_actionMap["AirAttack"] = ActionState::AirAttack;
	s_actionMap["LightAttack"] = ActionState::LightAttack;
	s_actionMap["UpLightAttack"] = ActionState::UpLightAttack;
	s_actionMap["HeavyAttack"] = ActionState::HeavyAttack;

	s_actionMap["Alert"] = ActionState::Alert;
	s_actionMap["Hurt"] = ActionState::Hurt;
	s_actionMap["Dead"] = ActionState::Dead;

	s_actionMap["IdleToWalk"] = ActionState::IdleToWalk;
	s_actionMap["WalkToIdle"] = ActionState::WalkToIdle;

	s_actionMap["IdleToRun"] = ActionState::IdleToRun;
	s_actionMap["WalkToRun"] = ActionState::WalkToRun;
	s_actionMap["RunToIdle"] = ActionState::RunToIdle;
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