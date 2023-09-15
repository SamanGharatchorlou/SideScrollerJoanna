#pragma once

#include "SRAC/Game/GameStates/State.h"

struct Player;

class GameState : public State
{
public:
	void init() override;
	void Update(float dt) override;
	void fastUpdate(float dt) override;
	void handleInput() override;
	
	void pause() override;
	void resume() override;
	void exit() override;


private:
	void initCamera();
	//void initRendering();

	Player* player;

};
