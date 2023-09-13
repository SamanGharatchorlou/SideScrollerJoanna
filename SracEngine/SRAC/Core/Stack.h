#pragma once

template<class T>
class StateMachine
{
public:
	void Push(const T& item)
	{
		mTopIndex++;
		mStack[mTopIndex] = item;
	}

	void Pop()
	{
		if (mTopIndex > 0)
			mTopIndex--;
	}

	T& Top()
	{
		return mStack[mTopIndex];
	}

	void ProcessStateChanges()
	{
		if (pop && add) // replace
		{
			mStack[mTopIndex]->OnExit();
			mStack[mTopIndex] = mStateToAdd
			mStack[mTopIndex]->OnEnter();
		}
	}


private:
	int mTopIndex;
	T mStack[8];

	T mStateToAdd;

	bool pop;
	bool add;
};