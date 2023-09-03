#pragma once

// class to be inherited by all game state objects
struct State
{
	virtual ~State() { }

	// pure virtual functions - require override
	virtual void init() { } ;
	virtual void handleInput() { };

	// State changes etc.
	virtual void slowUpdate(float dt) { };

	// Movement, collisions etc 
	virtual void fastUpdate(float dt) { };

	virtual void exit() { };

	// optional functions
	virtual void pause() {  }
	virtual void resume() {  }
};


// The null state does nothing
struct NullState : public State
{
	void init() override {}
	void handleInput() override {}
	void slowUpdate(float /*dt*/) override { /* do nothing */ }
	void fastUpdate(float /*dt*/) override { /* do nothing */ }
	void exit() override { 
		DebugPrint(Warning, "Removing the null state"); 
	}
};