#pragma once
#include "projects/Shared/SteeringAgent.h"
#include "projects/App_AmbientInteractions/Role.h"

class NpcAgent final : public SteeringAgent
{
public:
	NpcAgent(Role::Name canAssumeRoles) : SteeringAgent(), m_CanAssumeRoles{canAssumeRoles} {};
	virtual ~NpcAgent() = default;

	bool IsAvailable() { return m_IsAvailable; }
	void Acquire() { m_IsAvailable = false; }
	void Release() { m_IsAvailable = true; }

	bool CanAssumeRole(Role::Name roleName);
private:
	bool m_IsAvailable = true;
	Role::Name m_CanAssumeRoles{};
};

