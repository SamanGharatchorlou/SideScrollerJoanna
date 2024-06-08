#pragma once

#include "SRAC/Game/GameStates/State.h"

struct Player;

class GameState : public State
{
public:
	void Init() override;
	void Update(float dt) override;
	void FastUpdate(float dt) override;
	void HandleInput() override;
	
	void Pause() override;
	void Resume() override;
	void Exit() override;


private:
	void initCamera();
	//void initRendering();

	Player* player;

};
