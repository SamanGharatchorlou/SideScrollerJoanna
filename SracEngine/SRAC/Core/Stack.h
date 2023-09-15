#pragma once

template<class T>
class ActionStack
{
public:
	static const u32 Capacity() { return 7; }

	void Push(const T& item)
	{
		if ( (idx + 1) < Capacity() )
		{
			add = true;
			stateToAdd = item;
		}
	}

	void Pop()
	{
		if (idx >= 0)
			pop = true;
	}

	void Replace(const T& item)
	{
		Pop();
		add(item);
	}

	T& Top() { return stack[idx]; }
	const T& Top() const { return stack[idx]; }

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
			stack[idx]->init();
			stack[idx] = stateToAdd;
			stack[idx]->exit();
			stateFrames = 0;
		}
		else if (pop)
		{
			stack[idx]->init();
			idx--;
			stateFrames = 0;
		}
		else if(add)
		{
			idx++;
			stack[idx] = stateToAdd;
			stateFrames = 0;
		}
	}


	T stack[7];
	T stateToAdd;

	u32 stateFrames;
	int idx;
	bool pop;
	bool add;
};