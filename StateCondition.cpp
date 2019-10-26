// StateCondition.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <ratio>
#include "BaseStateManager.h"
#include "TestStateManager.h"
#include "TestAnimationManager.h"

double getTimeinMS()
{
	auto int_ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
	return int_ms2.count();
}

int main()
{
	// Performance + 2 state switch test
	{
		sGlobalContext context;
		CTestManager* mgr = new CTestManager();

		mgr->setState(context, eTestStates::FIRST);

		auto start = getTimeinMS();

		for (int i = 0; i < 100; i++)
		{
			mgr->update(0, context);
			//std::cout << "State: " << mgr->getActiveState() << std::endl;
			//std::cout << "i: " << context.i<< std::endl;
		}
		std::cout << "Total entries: " << mgr->totalEntries << "in: " << (getTimeinMS() - start) << "ms" << std::endl;
	}

	// Animation state test
	{
		CFakeEntity context;
		CAnimationStateManager* mgr = new CAnimationStateManager();

		mgr->setState(context, eAnimationStates::IDLE);

		bool slowDown = false;
		for (int i = 0; i < 100; i++)
		{
			if (slowDown && context.velocityComponent <= 0.0) // Slow down to stom
				slowDown = false;
			if(context.velocityComponent > 1.0f) // Start slowing when reach 1.0 vel
				slowDown = true;
			if (context.velocityComponent < 1.0f and !slowDown) // Speed up when now slowing down
			{
				context.velocityComponent += 0.033;
			}
			else // Slow down
				context.velocityComponent -= 0.033;

			mgr->update(0.1, context);
		}
		std::cout << "Total entries: " << mgr->totalEntries << std::endl;
	}



    std::cout << "Hello World!\n";
}