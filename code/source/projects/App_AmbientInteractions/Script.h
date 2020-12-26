#pragma once
#include "framework/EliteAI/EliteData/EBlackboard.h"
#include <projects/App_AmbientInteractions/Role.h>

class Script
{
public:
	Script() : m_IsRunning{ false } {};
	virtual ~Script();
	Script(const Script&) = delete;
	Script& operator=(const Script&) = delete;
	Script(Script&&) = delete;
	Script& operator=(Script&&) = delete;

	virtual bool IsPreconditionMet(Elite::Blackboard* pBlackboard);
	virtual bool IsEndConditionMet();

	virtual void Start(Elite::Blackboard* pBlackboard);

	virtual bool Update(float deltaTime);	//Returns wether script should be updated. (Used in derived classes)
	
	virtual void End();
	virtual void OnError();
protected:
	Elite::Blackboard* m_pBlackboard = nullptr;
	std::vector<Role> m_Roles{};
	bool AreAllRolesMet(Elite::Blackboard* pBlackboard);
	virtual bool RoleAllocation() = 0;

	bool m_IsRunning = false;
	float m_TimeElapsed = 0.f;
	float m_Cooldown = 10.f;
	float m_CooldownRemaining = 0.f;
};

