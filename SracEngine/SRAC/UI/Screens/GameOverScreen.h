#pragma once

#include "Screen.h"


class GameOverScreen : public Screen
{
public:
	GameOverScreen() { }

	void init() override;
	void Update() override;
	void exit() override { };

	UIScreen::Type type() override { return UIScreen::Type::GameOver; }

	int mMapLevel;
	int mScore;
	int mKills;
	StringBuffer32 mHealth;

private:
	void setTextBox(const char* textbox, int value);
};