#pragma once

#include "SRAC/Game/GameStates/State.h"

class PauseState : public State
{
public:
	void init() override;
	void Update(float dt) override;
	void fastUpdate(float dt) override { };
	void handleInput() override;

	void exit() override;
};