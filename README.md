# StateCondition
Conditional state machine template library

* Enum state          - Unique state identifyer
* State data          - Static state data used for manipulation and conditional comparision.
* Manipulated Context - Dynamic state data manipulated on entry and update.

* Transition condition - State change condition, If true, state is switched to target state, activating CBaseManager::onEntry event.
* solveMultiples       - Virtual function that can be overloaded to provide custom functionality in case where more than 1 exit condition is true. Default solveMultiples picks first state.




Examples see TestStateManager.h  
Create Enum state type
```
enum eTestStates
{
	NONE = 0,
	FIRST,
	SECOND
};
```

Create State data type
```
struct sTestData
{
	int threshold = 0;
	double countInterval = 0;
};
```

Create Manipulated context type
```
struct sGlobalContext
{
	int i = 0;
}
```

Create class derived from CBaseStateManager using defined types
```
class CTestManager : public CBaseStateManager<sGlobalContext, eTestStates, sTestData>
{
};
```

Overload and define onEntry function. Use getData(state) and manipulate context on entry to newState
```
virtual void onEntry(sGlobalContext& context, eTestStates oldState, eTestStates newState)
{
  // ... code here ...
}
```

Overload and define update function.
```
virtual void update(double dt, sGlobalContext& context)
{
  CBaseStateManager<sGlobalContext, eTestStates, sTestData>::update(dt, context);
  //... code here ...
}
```


* Set state data and conditional entries
set state data, is static throughout object lifetime. unless new setData is called
```
setData(eTestStates::FIRST, { 0, 1.0 });
setData(eTestStates::SECOND, { 10, -1.0 });
```

* add exit/entry condition between First and second state
Each update cycle, exit conditions are checked for current active state
This call is not allowed to manipulate data.
```
  auto lambda = [](const sGlobalContext& context, const sTestData& from, const sTestData& target) -> const bool{ return false };
	addConditionalEntry(eTestStates::FIRST, eTestStates::SECOND, lambda);
```
Given lambda never returns true, so Entry from first to second state is not possible.
