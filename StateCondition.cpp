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

		for (int i = 0; i < 1000000; i++)
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
			//std::cout << "State: " << mgr->getActiveState() << std::endl;
			//std::cout << "i: " << context.i<< std::endl;
		}
		std::cout << "Total entries: " << mgr->totalEntries << std::endl;
	}



    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
