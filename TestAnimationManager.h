#pragma once

#include "BaseStateManager.h"
#include <iostream>
#include <cmath>

class CAnimator
{
public:
	std::string playingAnimation = "";
	double		currentAnimationTime = 0.0f;
	double		playbackSpeed = 1.0f;
	double		animationLength = 0.0f;

	void update(double dt)
	{
		currentAnimationTime += dt * playbackSpeed;
		currentAnimationTime = std::fmod(currentAnimationTime, animationLength);
	}
};

//Marks current state
enum class eAnimationStates
{
	NONE = 0,
	IDLE,
	WALK
};

struct sAnimationData
{
	std::string animation		= "";
	double		animationSpeed	= 0.0f;
	double		animationLength = 0.0f;

};
struct CFakeEntity
{
	double velocityComponent = 0.0;
	CAnimator animatorComponent;
	double position = 0.0;
};

class CAnimationStateManager : public CBaseStateManager<CFakeEntity, eAnimationStates, sAnimationData>
{
private:

public:
	int totalEntries = 0;
	CAnimationStateManager()
	{
		setData(eAnimationStates::IDLE, sAnimationData{"Animation_Idle", 1.0f, 4.50f});
		setData(eAnimationStates::WALK, sAnimationData{"Animation_Walk", 1.0f, 1.0f });

		addConditionalEntry(eAnimationStates::IDLE, eAnimationStates::WALK, [](const CFakeEntity& context, const sAnimationData& from, const sAnimationData& target) -> const bool
			{
				return (context.velocityComponent > 0.1);
			});
		addConditionalEntry(eAnimationStates::WALK, eAnimationStates::IDLE, [](const CFakeEntity& context, const sAnimationData& from, const sAnimationData& target) -> const bool
			{
				return (context.velocityComponent < 0.1);
			});
	} 
	
	virtual void onEntry(CFakeEntity& context, eAnimationStates oldState, eAnimationStates newState)
	{
		const auto stateData = getStateData(newState);
		context.animatorComponent.currentAnimationTime = 0.0f;
		context.animatorComponent.playbackSpeed = stateData.animationSpeed;
		context.animatorComponent.playingAnimation = stateData.animation;
		context.animatorComponent.animationLength = stateData.animationLength;

		std::cout << "Entry: " << context.animatorComponent.playingAnimation << std::endl;
		totalEntries++;
	}

	//Every frame do something with active data
	virtual void update(double dt, CFakeEntity& e)
	{
		CBaseStateManager<CFakeEntity, eAnimationStates, sAnimationData>::update(dt, e);
		e.animatorComponent.update(dt);

		std::cout << "Play: " << e.animatorComponent.playingAnimation << " @ " << e.animatorComponent.currentAnimationTime << " Velocity: " << e.velocityComponent << std::endl;
	}
};