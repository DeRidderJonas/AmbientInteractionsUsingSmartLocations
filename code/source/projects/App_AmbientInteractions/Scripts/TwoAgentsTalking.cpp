#include "stdafx.h"
#include "TwoAgentsTalking.h"
#include "projects/App_AmbientInteractions/NpcAgent.h"

TwoAgentsTalking::TwoAgentsTalking()
    : Script()
{
    m_Roles.push_back({ Role::Name::AnyHuman, Role::Flag::LeaveAfterEnd, 2 }); //NPCs needed: 2 or 3 human npcs. All must leave after End()
    m_Roles.push_back({ Role::Name::AnyHuman, Role::Flag::LeaveAfterEnd | Role::Flag::DynamicJoin, 1 });
}

bool TwoAgentsTalking::IsPreconditionMet(Elite::Blackboard* pBlackboard)
{
    if (!Script::IsPreconditionMet(pBlackboard)) return false;

    return Script::AreAllRolesMet(pBlackboard);
}

void TwoAgentsTalking::Start(Elite::Blackboard* pBlackboard)
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

bool TwoAgentsTalking::Update(float deltaTime)
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

void TwoAgentsTalking::End()
{
    Script::End();
}

bool TwoAgentsTalking::IsEndConditionMet()
{
    if (!Script::IsEndConditionMet()) return false;

    return m_TimeElapsed >= m_AmountOfSecondsTalking;
}
