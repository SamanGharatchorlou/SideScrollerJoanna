#pragma once

#include "Screen.h"

class LoadingScreen : public Screen
{
public:
	LoadingScreen() { }

	void init() override { }
	void handleInput(const InputManager* input) { };
	void Update() override { };
	void exit() override { }

	UIScreen::Type type() override { return UIScreen::Type::LoadingScreen; }
};