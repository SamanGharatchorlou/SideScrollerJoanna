#pragma once

#include "UIBox.h"
#include "Input/Button.h"

class Texture;

class UIButton : public UIBox
{
public:
	enum State
	{
		None,
		Hovering,
		Pressed,
		Active
	};

	UIButton();
	UIButton(const StringMap32& attributes);
	~UIButton() { }

	void setTextures(Texture* defaultTexture, Texture* selected, Texture* hovering);

	void reset();

	bool handleInput(const InputManager* input) override;

	void setState(State state);
	void setHeld(bool isHeld);
	void setPressed(bool isPressed);
	void setReleased(bool isReleased);

	inline bool isPressed() const { return mButton.isPressed(); }
	inline bool isReleased() const { return mButton.isReleased(); }
	inline bool isHeld() const { return mButton.isHeld(); }
	inline int holdCount() const { return mButton.getHeldFrames(); }

	virtual UIType type() const override { return UIType::Button; }

	void setActive(bool active) { mIsActive = active; }
	bool isActive() const { return mIsActive; }

private:
	Texture* mDefault;
	Texture* mSelected;
	Texture* mHovered;

	Button mButton;

	bool mIsActive;
};
