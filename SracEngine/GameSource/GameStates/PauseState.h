#pragma once

#include "SRAC/Game/GameStates/State.h"

class PauseState : public State
{
public:
	void Init() override;
	void Update(float dt) override;
	void FastUpdate(float dt) override { };
	void HandleInput() override;

	void Exit() override;
};