# StateCondition
Conditional state machine template library

* Enum state          - Unique state identifyer
* State data          - Static state data used for manipulation and conditional comparision.
* Manipulated Context - Dynamic state data manipulated on entry and update.

* Transition condition - State change condition, If true, state is switched to target state, activating CBaseManager::onEntry event.
* solveMultiples       - Virtual function that can be overloaded to provide custom functionality in case where more than 1 exit condition is true. Default solveMultiples picks first state.