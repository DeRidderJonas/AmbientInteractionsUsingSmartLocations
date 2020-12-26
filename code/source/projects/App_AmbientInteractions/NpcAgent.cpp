#include "stdafx.h"
#include "NpcAgent.h"

bool NpcAgent::CanAssumeRole(Role::Name roleName)
{
	if (!m_IsAvailable) return false;

	Role::Name compatible{ m_CanAssumeRoles & roleName };
	//If npc is compatible with any of the roles, those role's bits will be set
	return int(compatible) > 0;
}
