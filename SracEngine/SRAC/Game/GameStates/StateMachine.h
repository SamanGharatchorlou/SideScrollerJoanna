#pragma once

#include <stack>

template<class T>
class StateMachine
{
public:
	StateMachine(T* nullState, bool owns_states = true);
	~StateMachine();

	void shallowClear(); // leaves the base null state
	//T* forcePop(); // Warning: does not call exit or resume

	void addState(T* state);
	void popState();
	void replaceState(T* state);

	T& getActiveState() const;
	node<T*>* getActiveStateNode() const;

	size_t size() const { return states.size(); }

private:
	//std::stack<T*> states;
	LinkedList<T*> states;
	bool mOwnsStates;
};


template<class T>
StateMachine<T>::StateMachine(T* nullState, bool owns_states) : mOwnsStates(owns_states)
{
	states.insert_back(nullState);
}


template<class T>
StateMachine<T>::~StateMachine()
{
	while (states.size() > 0)
	{
		if(mOwnsStates)
			delete states.back();

		states.pop_back();
	}
}


template<class T>
void StateMachine<T>::shallowClear()
{
	// Keep the NULL state at index 0
	while (states.size() > 1)
	{
		states.back()->exit();

		if(mOwnsStates)
			delete states.back();

		states.pop_back();
	}
}


template<class T>
void StateMachine<T>::addState(T* state)
{
	states.back()->pause();

	states.insert_back(state);
	states.back()->init();
}


template<class T>
void StateMachine<T>::popState()
{
	if (states.size() > 1)
	{
		T* popped_state = states.back();
		popped_state->exit();
		states.pop_back();

		states.back()->resume();

		if(mOwnsStates)
			delete popped_state;
	}
	else
	{
		DebugPrint(Error, "Cannot remove state, stack size = %d", states.size());
	}
}


template<class T>
void StateMachine<T>::replaceState(T* state)
{
	if (states.size() > 1)
	{
		T* popped_state = states.back();
		popped_state->exit();
		states.pop_back();

		states.insert_back(state);
		states.back()->init();

		if (mOwnsStates)
			delete popped_state;
	}
	else
	{
		DebugPrint(Error, "Cannot replace state, stack size = %d", states.size());
	}
}

template<class T>
T& StateMachine<T>::getActiveState() const
{
	return *states.back();
}


template<class T>
node<T*>* StateMachine<T>::getActiveStateNode() const
{
	return states.get_tail();
}
