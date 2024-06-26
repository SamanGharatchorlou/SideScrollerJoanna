#pragma once

#if UI_EDITOR
#include "Input/Button.h"

class ScreenController;
class InputManager;


class UIEditor
{
public:
	void handleInput(InputManager* input);

	BasicString selectElement(const InputManager* input);
	VectorF moveElement(const InputManager* input, Button::Key key, VectorF direction);

	void handleEvent(const InputManager* input,VectorF movement);

	void render();

private:
	void printPosition();


private:
	BasicString mElementID;

	float movementSpeed = 2.5f; // pixels moved per press
	int holdSpeed = 2; // larger is slower, using % for it
	int holdDelay = 15;
};
#endif