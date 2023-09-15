#pragma once

template<class T>
class ActionStack
{
public:
	static const u32 capacity() const { return 7; }

	void Push(const T& item)
	{
		if ( (idx + 1) < capacity() )
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

	void replace(const T& item)
	{
		pop();
		add(item);
	}

	T& Top()
	{
		return stack[idx];
	}

	void ProcessStateChanges()
	{
		stateFrames++;

		if (pop && add) // replace
		{
			stack[idx]->OnExit();
			stack[idx] = stateToAdd;
			stack[idx]->OnEnter();
			stateFrames = 0;
		}
		else if (pop)
		{
			stack[idx]->OnExit();
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


private:
	T stack[7];
	T stateToAdd;

	u32 stateFrames;
	int idx;
	bool pop;
	bool add;
};