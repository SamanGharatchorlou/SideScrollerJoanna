#pragma once

#include "Screen.h"

class Texture;

class MainMenuScreen : public Screen
{
public:
	MainMenuScreen() { };

	void init() override;

	//void updateInputs(const InputManager* input) override;
	void handleInput(const InputManager* input) { };
	void Update() override;

	//void resume() override;
	//void pause() override;
	void exit() override { };

	UIScreen::Type type() override { return UIScreen::Type::MainMenu; }

	//void setCursorTexture(Texture* texture);
};