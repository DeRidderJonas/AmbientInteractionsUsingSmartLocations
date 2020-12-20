#include "stdafx.h"
#include "TwoAgentsTalking.h"
#include "projects/App_AmbientInteractions/NpcAgent.h"

bool TwoAgentsTalking::IsPreconditionMet(Elite::Blackboard* pBlackboard)
{
    if (m_IsRunning) return false;

    std::vector<NpcAgent*>* pAgents = nullptr;
    bool dataAvailable = pBlackboard->GetData("agents", pAgents);
    if(!dataAvailable) return false;
    
    const int amountOfAgentsNeeded{ 2 };
    return pAgents->size() >= amountOfAgentsNeeded;
}

void TwoAgentsTalking::Start(Elite::Blackboard* pBlackboard)
{
    Script::Start(pBlackboard);
    std::vector<NpcAgent*>* pAgents = nullptr;
    pBlackboard->GetData("agents", pAgents);

    size_t randAgent0{ rand() % pAgents->size() };
    NpcAgent* agent0{ pAgents->at(randAgent0) };
    size_t randAgent1{ rand() % pAgents->size() };
    while (randAgent1 == randAgent0) randAgent1 = rand() % pAgents->size();
    NpcAgent* agent1{ pAgents->at(randAgent1) };

    agent0->Acquire();
    agent1->Acquire();

    agent0->SetToSeek();
    agent1->SetToSeek();

    m_pBlackboard->AddData("agent0", agent0);
    m_pBlackboard->AddData("agent1", agent1);
}

bool TwoAgentsTalking::Update(float deltaTime)
{
    if(!Script::Update(deltaTime)) return false;

    NpcAgent* agent0 = nullptr;
    NpcAgent* agent1 = nullptr;
    m_pBlackboard->GetData("agent0", agent0);
    m_pBlackboard->GetData("agent1", agent1);

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

    agent0->Release();
    agent1->Release();

    Script::End();
}
