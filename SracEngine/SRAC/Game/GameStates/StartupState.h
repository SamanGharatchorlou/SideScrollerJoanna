#pragma once

#include "SRAC/Game/GameStates/State.h"
#include "Game/Data/LoadingManager.h"

//#include <thread>

struct GameData;

class StartupState : public State
{
public:
	StartupState(GameData& gd);
	void init() override;
	void Update(float dt) override;
	void exit() override;

private:
	void handleInput() override { };
	void fastUpdate(float dt) override { };

	TimerF timer;
};