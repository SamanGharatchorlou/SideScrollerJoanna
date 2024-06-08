#pragma once

#include "SRAC/Game/GameStates/State.h"
#include "Game/Data/LoadingManager.h"

//#include <thread>

struct GameData;

class StartupState : public State
{
public:
	StartupState(GameData& gd);
	void Init() override;
	void Update(float dt) override;
	void Exit() override;

private:
	void HandleInput() override { };
	void FastUpdate(float dt) override { };

	TimerF timer;
};