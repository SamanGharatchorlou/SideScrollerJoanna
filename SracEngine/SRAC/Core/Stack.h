#pragma once

template<class T>
class ActionStack
{
public:
	ActionStack()
	{
		memset(stack, Capacity(), Capacity() * sizeof(T*));
	}

	static const u32 Capacity() { return 8; }
	bool HasAction() const { return idx != -1; }

	void Push(T* item)
	{
		if ( (idx + 1) < Capacity() )
		{
			add = true;
			stateToAdd = item;
		}
		else
		{
			DebugPrint(PriorityLevel::Error, "Action stack full, cannot push another action");
		}
	}

	void Pop()
	{
		if (idx >= 0)
			pop = true;
	}

	void Replace(T* item)
	{
		Pop();
		Push(item);
	}

	T& Top() { return *stack[idx]; }
	const T& Top() const { return *stack[idx]; }

	const T& Previous() const { 
		if(idx > 0)
			return stack[idx - 1];
		return stack[idx];
	}

	void ProcessStateChanges(bool can_change = true)
	{
		stateFrames++;

		if(!can_change)
		{
			pop = false;
			add = false;
		}

		if (pop && add) // replace
		{
			stack[idx]->Init();
			stack[idx] = stateToAdd;
			stack[idx]->Exit();
			stateFrames = 0;
		}
		else if (pop)
		{
			stack[idx]->Init();
			idx--;
			stateFrames = 0;
		}
		else if(add)
		{
			idx++;
			stack[idx] = stateToAdd;
			stateFrames = 0;
		}

		pop = false;
		add = false;
	}


	T* stack[8];
	T* stateToAdd = nullptr;

	u32 stateFrames = 0;
	u32 idx = -1;
	bool pop = false;
	bool add = false;
};