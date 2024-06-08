#pragma once

template<class T>
class ActionStack
{
public:
	bool HasAction() const { return stack.size() > 0; }

	void Push(T* item)
	{
		stateToAdd = item;
	}

	void Pop()
	{
		pop = true;
	}

	void Replace(T* item)
	{
		Pop();
		Push(item);
	}

	T& Top() { return *stack.back(); }
	const T& Top() const { return *stack.back(); }

	const T* Previous() const 
	{ 
		u32 size = stack.size();
		if(size > 1)
			return stack[size - 2];

		return nullptr;
	}

	void ProcessStateChanges()
	{
		stateFrames++;

		if(pop && stack.size() > 0)
		{
			stack.back()->Exit();
			stack.pop_back();

			stateFrames = 0;
		}

		if(stateToAdd)
		{
			stack.push_back(stateToAdd);
			stack.back()->Init();

			stateFrames = 0;
			stateToAdd = nullptr;
		}

		pop = false;
	}

	std::vector<T*> stack;

	T* stateToAdd = nullptr;

	u32 stateFrames = 0;
	bool pop = false;
};