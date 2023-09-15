#pragma once

#include "Screen.h"

class Texture;

class GameScreen : public Screen
{
public:
	GameScreen();

	void init() override;

	void handleInput(const InputManager* input);
	void Update() override;
	void exit() override;

	void resume() override;
	void pause() override;

	UIScreen::Type type() override { return UIScreen::Type::Game; }

	void setCursorTexture(Texture* texture);
};