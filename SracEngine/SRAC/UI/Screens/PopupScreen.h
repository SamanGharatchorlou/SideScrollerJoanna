#pragma once

#include "Screen.h"

class PopupScreen : public Screen
{
public:
	PopupScreen() { }

	void build(const char* title, const char* body);

	void init() override { }
	void handleInput(const InputManager* input);
	void slowUpdate() override;
	void exit() override { }

	UIScreen::Type type() { return UIScreen::Type::Popup; }

	void setMainText(const char* text);
private:
	void setTitle(const char* title);
	void setBody(const char* body);

private:
	void exitPopup();

};