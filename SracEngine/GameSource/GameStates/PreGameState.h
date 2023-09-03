#pragma once

#include "SRAC/Game/GameStates/State.h"

class PreGameState : public State
{
public:
	void init() override;
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override { };
	void handleInput() override { };
	void exit() override;

	void resume() { };

private:
	BasicString mSelectedCharacter;
	BasicString mSelectedWeapon;
};