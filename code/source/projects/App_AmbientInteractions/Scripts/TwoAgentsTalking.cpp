#include "stdafx.h"
#include "TwoAgentsTalking.h"
#include "projects/App_AmbientInteractions/NpcAgent.h"

TwoAgentsTalking::TwoAgentsTalking()
    : Script()
{
    m_Roles.push_back({ Role::Name::AnyHuman, Role::Flag::LeaveAfterEnd, 2 }); //NPCs needed: 2 or 3 human npcs. All must leave after End()
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
        OnError();
        return;
    }

    NpcAgent* agent0 = nullptr;
    NpcAgent* agent1 = nullptr;
    m_pBlackboard->GetData("agent0", agent0);
    m_pBlackboard->GetData("agent1", agent1);

    agent0->SetToSeek();
    agent1->SetToSeek();

}

bool TwoAgentsTalking::Update(float deltaTime)
{
    if(!Script::Update(deltaTime)) return false;

    NpcAgent* agent0 = nullptr;
    NpcAgent* agent1 = nullptr;
    m_pBlackboard->GetData("agent0", agent0);
    m_pBlackboard->GetData("agent1", agent1);

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
    NpcAgent* agent0 = nullptr;
    NpcAgent* agent1 = nullptr;
    m_pBlackboard->GetData("agent0", agent0);
    m_pBlackboard->GetData("agent1", agent1);

    if (agent0)
    {
        agent0->Release();
        agent0->SetToWander();
    }
    if (agent1)
    {
        agent1->Release();
        agent1->SetToWander();
    }

    Script::End();
}

bool TwoAgentsTalking::IsEndConditionMet()
{
    if (!Script::IsEndConditionMet()) return false;

    return m_TimeElapsed >= m_AmountOfSecondsTalking;
}

bool TwoAgentsTalking::RoleAllocation()
{
    std::vector<NpcAgent*>* pAgents = nullptr;
    m_pBlackboard->GetData("agents", pAgents);

    //Shuffle all participants to counter any unintentional bias towards first or last NPC
    std::random_shuffle(pAgents->begin(), pAgents->end());

    bool agent0Chosen{ false };
    for (NpcAgent* pAgent : *pAgents)
    {
        if (pAgent->CanAssumeRole(m_Roles[0].GetNames()))
        {
            if (!agent0Chosen)
            {
                pAgent->Acquire();
                m_pBlackboard->AddData("agent0", pAgent);
                agent0Chosen = true;
                continue;
            }
            else 
            {
                pAgent->Acquire();
                m_pBlackboard->AddData("agent1", pAgent);
                return true;
            }
        }
    }
    
    
    return false;
}
