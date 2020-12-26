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

void AgentsTalking::Start(Elite::Blackboard* pBlackboard)
{
    Script::Start(pBlackboard);

    if (!m_IsRunning) return;

    std::vector<NpcAgent*> agents{ m_Roles[0].GetAgents() };
    Elite::Vector2 inBetween{};
    for (NpcAgent* pAgent : agents) inBetween += pAgent->GetPosition();
    inBetween /= float(agents.size());

    TargetData target{};
    target.Position = inBetween;
    for (NpcAgent* pAgent : agents)
    {
        pAgent->SetTarget(target);
        pAgent->SetToSeek();
    }

    m_pBlackboard->AddData("Target", target);
}

bool AgentsTalking::Update(float deltaTime)
{
    if(!Script::Update(deltaTime)) return false;

    std::vector<NpcAgent*> agents{ m_Roles[0].GetAgents() };
    agents.insert(agents.end(), m_Roles[1].GetAgents().begin(), m_Roles[1].GetAgents().end());

    if (agents.size() <= 0)
    {
        OnError();
        return false;
    }
    
    for (NpcAgent* pAgent : agents)
    {
        pAgent->SetBodyColor({ 0,0,1,1 });
    }

    return true;
}

void AgentsTalking::End()
{
    Script::End();
}

bool AgentsTalking::DynamicJoin()
{
    //When another agent joins, reset the timer to 0 (otherwise he might join just before the conversation ends)
    m_TimeElapsed = 0.f;

    bool dynamicJoinSuccess = Script::DynamicJoin();
    if (!dynamicJoinSuccess) return false;

    TargetData target{};
    m_pBlackboard->GetData("Target", target);

    for (NpcAgent* pAgent : m_Roles[1].GetAgents())
    {
        pAgent->SetTarget(target);
        pAgent->SetToSeek();
    }
    return true;
}

bool AgentsTalking::IsEndConditionMet()
{
    if (!Script::IsEndConditionMet()) return false;

    return m_TimeElapsed >= m_AmountOfSecondsTalking;
}
