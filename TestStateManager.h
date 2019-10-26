#pragma once

#include "BaseStateManager.h"

//Marks current state
enum eTestStates
{
	NONE = 0,
	FIRST,
	SECOND
};

struct sTestData
{
	int threshold = 0;
	double countInterval = 0;
};
struct sGlobalContext
{
	int i = 0;
};

class CTestManager;
class CTestManager : public CBaseStateManager<sGlobalContext, eTestStates, sTestData>
{
private:
public:
	int totalEntries = 0;
	CTestManager()
	{
		m_ActiveState = eTestStates::FIRST;
		setData(eTestStates::FIRST, { 0, 1.0 }); // Entry cost -10
		setData(eTestStates::SECOND, { 10, -1.0 }); // entry cost 10


		//Each frame check if any exit conditions are true
		addConditionalEntry(eTestStates::FIRST, eTestStates::SECOND, [](sGlobalContext& context, sTestData& from, sTestData& target) -> bool
			{
				auto& i = context.i;
				if (i == target.threshold)
				{
					return true;
				}
				//if come global integer reaches threshold, do sth
				return false;
			});

		addConditionalEntry(eTestStates::SECOND, eTestStates::FIRST, [](sGlobalContext& context, sTestData& from, sTestData& target) -> bool
			{
				auto& i = context.i;
				if (i == target.threshold)
				{
					return true;
				}
				//if come global integer reaches threshold, do sth
				return false;
			});
	}


	virtual void onEntry(sGlobalContext& context, eTestStates oldState, eTestStates newState)
	{
		//std::cout << "Entry: " << newState << std::endl;
		totalEntries++;
	}

	//Every frame do something with active data
	virtual void update(double dt, sGlobalContext& context)
	{
		CBaseStateManager<sGlobalContext, eTestStates, sTestData>::update(dt, context); // adds total 13 - 15ms

		auto state = getActiveState();
		auto activeData = getStateData(state); // adds +3 ms, total up to 18

		context.i += activeData.countInterval;
	}
};
