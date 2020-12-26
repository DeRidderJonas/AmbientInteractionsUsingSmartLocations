#include "stdafx.h"
#include "AgentsTalking.h"
#include "projects/App_AmbientInteractions/NpcAgent.h"

AgentsTalking::AgentsTalking(size_t min_agents, size_t max_agents, size_t min_agents_dynamic, size_t max_agents_dynamic)
    : Script()
{
    m_Roles.push_back({ Role::Name::AnyHuman, Role::Flag::LeaveAfterEnd, min_agents, max_agents });
    m_Roles.push_back({ Role::Name::AnyHuman, Role::Flag::LeaveAfterEnd | Role::Flag::DynamicJoin, min_agents_dynamic, max_agents_dynamic });
}

AgentsTalking::AgentsTalking(size_t agents, size_t agents_dynamic)
    : AgentsTalking(agents, agents, agents_dynamic, agents_dynamic)
{
}

bool AgentsTalking::IsPreconditionMet(Elite::Blackboard* pBlackboard)
{
    if (!Script::IsPreconditionMet(pBlackboard)) return false;

    return Script::AreAllRolesMet(pBlackboard);
}

void AgentsTalking::Start(Elite::Blackboard* pBlackboard)
{
    Script::Start(pBlackboard);
    bool roleAllocationSuccess = RoleAllocation();
    if (!roleAllocationSuccess)
    {
        std::cout << "Role allocation failed" << '\n';
        OnError();
        return;
    }

    NpcAgent* agent0 = m_Roles[0][0];
    NpcAgent* agent1 = m_Roles[0][1];

    agent0->SetToSeek();
    agent1->SetToSeek();
}

bool AgentsTalking::Update(float deltaTime)
{
    if(!Script::Update(deltaTime)) return false;

    NpcAgent* agent0 = m_Roles[0][0];
    NpcAgent* agent1 = m_Roles[0][1];

    if (agent0 == nullptr || agent1 == nullptr)
    {
        OnError();
        return false;
    }

    Elite::Vector2 inBetween{ (agent0->GetPosition() + agent1->GetPosition()) / 2 };
    TargetData target{};
    target.Position = inBetween;
    agent0->SetTarget(target);
    agent1->SetTarget(target);

    agent0->SetBodyColor({ 0,0,1,1 });
    agent1->SetBodyColor({ 0,0,1,1 });

    return true;
}

void AgentsTalking::End()
{
    Script::End();
}

bool AgentsTalking::IsEndConditionMet()
{
    if (!Script::IsEndConditionMet()) return false;

    return m_TimeElapsed >= m_AmountOfSecondsTalking;
}
