#include "stdafx.h"
#include "NpcAgent.h"

bool NpcAgent::CanAssumeRole(Role::Name roleName)
{
	if (!m_IsAvailable) return false;

	Role::Name compatible{ m_CanAssumeRoles & roleName };
	//If npc is compatible with any of the roles, those role's bits will be set
	return int(compatible) > 0;
}

bool NpcAgent::IsAtTarget() const
{
	TargetData target{ m_pSteeringBehavior->GetTarget() };
	DEBUGRENDERER2D->DrawCircle(target.Position, 10.f, { 1,1,0 }, 0.7f);
	return Elite::AreEqual(target.Position.x, GetPosition().x, 2.f) && Elite::AreEqual(target.Position.y, GetPosition().y, 2.f);
}
