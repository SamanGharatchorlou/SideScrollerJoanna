#pragma once

#include "Screen.h"
#include "UI/Elements/UISlider.h"


class SettingsScreen : public Screen
{
public:
	SettingsScreen() { }

	void init() override;
	void slowUpdate() override;
	void exit() override { };

	UIScreen::Type type() override { return UIScreen::Type::Settings; }
};