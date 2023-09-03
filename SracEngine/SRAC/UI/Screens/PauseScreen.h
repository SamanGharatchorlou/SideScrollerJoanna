#pragma once

#include "Screen.h"

class PauseScreen : public Screen
{
public:
	enum Option
	{
		None,
		Resume,
		Settings,
		Restart,
		Quit
	};

public:
	PauseScreen() { }

	void init() override { }
	void handleInput(const InputManager* input);
	void slowUpdate() override;
	void exit() override { };

	UIScreen::Type type() override { return UIScreen::Type::Pause; }

private:
	void popScreenState();

};