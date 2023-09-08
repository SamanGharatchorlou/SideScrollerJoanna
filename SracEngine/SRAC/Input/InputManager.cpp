#include "pch.h"
#include "InputManager.h"

//#include "Button.h"
#include "Game/Camera/Camera.h"
#include "Debugging/ImGui/ImGuiMenu.h"

void InputManager::init()
{
	bindDefaultButtons();
}

InputManager* InputManager::Get()
{
	GameData& gd = GameData::Get();
	ASSERT(gd.inputManager != nullptr, "Audio manager has no been set up yet");
	return gd.inputManager;
}

void InputManager::processInputEvent(SDL_Event& event)
{
#if IMGUI
	bool has_input = DebugMenu::HandleInput(event);
	if (has_input)
		return;
#endif

	if (event.type == SDL_MOUSEMOTION)
		processMouseMovementEvent();
	else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
		processMouseButtonEvent(event);
	else
		processButtonEvent(event);
}


const Button& InputManager::getButton(Button::Key key) const
{
	for (unsigned int i = 0; i < mButtons.size(); i++)
	{
		if (mButtons[i].isKey(key))
			return mButtons[i];
	}

	DebugPrint(Warning, "Button for key %d not found, return button with KEY: NONE", key);
	return mButtons[0];
}


// Cursor
void InputManager::setCursorSize(VectorF size) { mCursor.setSize(size); }
bool InputManager::isCursorPressed(Cursor::ButtonType button) const { return mCursor.isPressed(button); }
bool InputManager::isCursorReleased(Cursor::ButtonType button) const { return mCursor.isReleased(button); }
bool InputManager::isCursorHeld(Cursor::ButtonType button) const { return mCursor.isHeld(button); }


// Must run before processInputEvent
void InputManager::resetInputEvents()
{
	// Reset cursor states
#if DEBUG_CHECK
	if (((mCursor.isHeld(Cursor::Left) || mCursor.isPressed(Cursor::Left)) && mCursor.isReleased(Cursor::Left)) ||
		((mCursor.isHeld(Cursor::Right) || mCursor.isPressed(Cursor::Right)) && mCursor.isReleased(Cursor::Right)))
	{
		DebugPrint(Warning, "Cursor is being pressed and released at the same time");
	}
#endif

	mCursor.clearInputs();

	// Reset button states
	for (Button& button : mButtons)
	{
#if DEBUG_CHECK
		if ((button.isHeld() || button.isPressed()) && button.isReleased())
		{
			DebugPrint(Warning, "Button key %d is being pressed and released at the same time", button.key());
		}
#endif

		button.setPressed(false);
		button.setReleased(false);

		//if (button.isHeld())
		//	button.incrementHeldFrames();
		//else
		//	button.setHeldFrames(0);
	}
}


InputManager createManager(const InputPacket& inputData)
{
	InputManager manager;
	manager.init();
	manager.resetInputEvents();
	return manager;
}


// --- Private Functions --- //

void InputManager::processMouseMovementEvent()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	float scale = Camera::Get()->scale();
	VectorF cursorPostion = VectorF((float)x, (float)y) / scale;

	mCursor.setPosition(cursorPostion);
	mCursor.setMotion(true);
}


void InputManager::processMouseButtonEvent(SDL_Event& event)
{
	SDL_MouseButtonEvent buttonEvent = event.button;

	Cursor::ButtonType buttonType;
	bool isHeld = false;
	bool isPressed = false;
	bool isReleased = false;

	if (buttonEvent.button == Button::LeftClick)
		buttonType = Cursor::Left;
	else if (buttonEvent.button == Button::rightClick)
		buttonType = Cursor::Right;
	else
		DebugPrint(Warning, "Mouse button type %d not left or right", buttonEvent.button);

	// Get input data
	Button cursorButton = mCursor.getButton(buttonType);

	if (!cursorButton.isHeld())
		isPressed = (event.type == SDL_MOUSEBUTTONDOWN);

	isHeld = (event.type == SDL_MOUSEBUTTONDOWN);
	isReleased = (event.type == SDL_MOUSEBUTTONUP);

	// Set input data
	//cursorButton.setHeld(isHeld);
	cursorButton.setPressed(isPressed);
	cursorButton.setReleased(isReleased);

	if (isPressed)
		cursorButton.mHeldFrames++;
	if (isReleased)
		cursorButton.mHeldFrames = 0;

	mCursor.setButton(buttonType, cursorButton);
}


void InputManager::processButtonEvent(SDL_Event& event)
{
	for (Button& button : mButtons)
	{
		if (button.isKey(event.key.keysym.sym))
		{
			if (!button.isHeld())
				button.setPressed(event.type == SDL_KEYDOWN);

			button.setReleased(event.type == SDL_KEYUP);
		}

		// todo: move this up into the if(b.iskey) scope
		if (button.mHeldFrames > 0)
			button.mHeldFrames++;
		else if (button.isPressed())
			button.mHeldFrames = 1;

		if (button.isReleased())
			button.mHeldFrames = 0;
	}
}


void InputManager::bindDefaultButtons()
{
	mButtons.push_back(Button(Button::Key::None));

	// Movement
	mButtons.push_back(Button(Button::Key::Up));
	mButtons.push_back(Button(Button::Key::Down));
	mButtons.push_back(Button(Button::Key::Left));
	mButtons.push_back(Button(Button::Key::Right));

	mButtons.push_back(Button(Button::Key::LeftArrow));
	mButtons.push_back(Button(Button::Key::RightArrow));
	mButtons.push_back(Button(Button::Key::DownArrow));
	mButtons.push_back(Button(Button::Key::UpArrow));

	// Game state
	mButtons.push_back(Button(Button::Key::Pause));
	mButtons.push_back(Button(Button::Key::Esc));
	mButtons.push_back(Button(Button::Key::Quit));
	mButtons.push_back(Button(Button::Key::Enter));
	mButtons.push_back(Button(Button::Key::Ctrl));
	mButtons.push_back(Button(Button::Key::Space));
	mButtons.push_back(Button(Button::Key::Shift));

	// Keys
	mButtons.push_back(Button(Button::Key::E));

	// Numbers
	mButtons.push_back(Button(Button::Key::Zero));
	mButtons.push_back(Button(Button::Key::One));
	mButtons.push_back(Button(Button::Key::Two));
	mButtons.push_back(Button(Button::Key::Three));
	mButtons.push_back(Button(Button::Key::Four));
	mButtons.push_back(Button(Button::Key::Five));
	mButtons.push_back(Button(Button::Key::Six));
	mButtons.push_back(Button(Button::Key::Seven));
	mButtons.push_back(Button(Button::Key::Eight));
	mButtons.push_back(Button(Button::Key::Nine));
}
