#pragma once
#include "framework/EliteAI/EliteData/EBlackboard.h"

class Script
{
public:
	Script() : m_IsRunning{ false } {};
	virtual ~Script();
	Script(const Script&) = delete;
	Script& operator=(const Script&) = delete;
	Script(Script&&) = delete;
	Script& operator=(Script&&) = delete;

	virtual bool IsPreconditionMet(Elite::Blackboard* pBlackboard) = 0;
	virtual bool IsEndConditionMet() = 0;
	virtual void Start(Elite::Blackboard* pBlackboard);
	//Returns wether script should be updated. (Used in derived classes)
	virtual bool Update(float deltaTime);
	virtual void End();
protected:
	Elite::Blackboard* m_pBlackboard = nullptr;
	bool m_IsRunning = false;
	float m_TimeElapsed = 0.f;
};

