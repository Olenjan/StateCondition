#pragma once

#include <stdexcept>
#include <map>
#include <unordered_map>
#include <vector>

template<typename C, typename T, typename D>
class CBaseStateManager
{
protected:
	struct sConditionalTarget
	{
		bool (*condition)(C& cd, D& current, D& target) = nullptr;
		T target;
	};
private:
	// Todo: Optimize out map. is slow.
	std::map<T, std::vector<sConditionalTarget>> m_Conditions;
	std::map<T, D>								 m_TransitionData;

protected:
	T											 m_ActiveState;

public:
	CBaseStateManager()
	{
	}

	void setData(T to, D data)
	{
		m_TransitionData[to] = data;
	}

	T getActiveState()
	{
		return m_ActiveState;
	}

	D getStateData(T to)
	{

		auto fromFound = m_TransitionData.find(to);

		if (fromFound != m_TransitionData.end())
		{
			auto foundData = fromFound->second;
			return foundData;
		}

		//Todo: custom exception
		std::string error = "CBaseStateManager: Given Transition data is not set.";
		std::cout << error << std::endl;
		throw std::runtime_error(error.c_str());
	}



	//
	void            addConditionalEntry(T state,
		T target,
		bool (*condition)(C& cd, D& current, D& target))
	{
		m_Conditions[state].push_back({ condition, target });
	}

	std::vector<T> getTrueStates(C& e)
	{
		std::vector<T> result;

		auto found = m_Conditions.find(m_ActiveState);
		if (found != m_Conditions.end())
		{
			//So far adds 1ms per 1m, with 2 states
			
			for (const auto& c : found->second)
			{

				auto current = getStateData(found->first);	// Adds +4ms total 8ms
				auto target = getStateData(c.target);		// Adds total +3ms, total 11ms
				
				if (c.condition(e, current, target))		// adds 2ms, total 13ms
				{
					result.push_back(c.target);
				}
			}
			
		}

		return result;
	}

	virtual void onEntry(C& controlledData, T oldState, T newState) = 0;

	T solveMultiples(std::vector<T> states)
	{
		if (states.size() == 0)
		{
			std::string error = "CBaseStateManager Unable to solve 0 states.";
			std::cout << error << std::endl;
			throw std::runtime_error(error.c_str());
		}

		T state = *states.begin();
		return state;
	}

	virtual void update(double dt, C& controlledData)
	{
		auto oldState = m_ActiveState;
		auto possibleStates = getTrueStates(controlledData);

		if (possibleStates.size() > 0) // somehow adds even more delay, up to 15ms
		{
			setState(controlledData, solveMultiples(possibleStates));
		}
	}

	//   
	void        setState(C& controlledData, T state)
	{
		auto oldState = m_ActiveState; // total 15 - 18ms
		m_ActiveState = state; //change in state // total 30 - 345 ms
		
		if (oldState != m_ActiveState)
		{
			onEntry(controlledData, oldState, m_ActiveState);
		}
	}
};