#include "stdafx.h"
#include "DancingHumans.h"
#include "projects/App_AmbientInteractions/NpcAgent.h"

DancingHumans::DancingHumans(size_t min_agents, size_t max_agents, size_t min_agents_dynamic, size_t max_agents_dynamic)
{
	m_Roles.push_back({ Role::Name::AnyHuman , Role::Flag::LeaveAfterEnd, min_agents, max_agents }); //Dancers
	m_Roles.push_back({ Role::Name::Tourist, Role::Flag::DynamicJoin | Role::Flag::LeaveAfterEnd, min_agents_dynamic, max_agents_dynamic }); //Audience
}

DancingHumans::DancingHumans(size_t agents, size_t agents_dynamic)
	: DancingHumans(agents, agents, agents_dynamic, agents_dynamic)
{}

bool DancingHumans::IsEndConditionMet()
{
	if (!Script::IsEndConditionMet()) return false;
	
	return m_TimeElapsed >= m_AmountOfSecondsToDance;
}

void DancingHumans::Start(Elite::Blackboard* pBlackboard)
{
	Script::Start(pBlackboard);

	if (!m_IsRunning) return;

	std::vector<NpcAgent*> dancers{ m_Roles[0].GetAgents() };
	
	float angleBetweenAgents{ 6.28f / dancers.size() };
	m_pBlackboard->AddData("Angle", angleBetweenAgents);
	float currAngle{ 0 };
	Elite::Vector2 center{};
	m_pBlackboard->GetData("location", center);

	for (NpcAgent* pAgent : dancers)
	{
		Elite::Vector2 posOnCircle{ cos(currAngle) * m_DanceCircleRadius, sin(currAngle) * m_DanceCircleRadius };
		pAgent->SetToSeek();
		pAgent->SetTarget(center + posOnCircle);

		currAngle += angleBetweenAgents;
	}

	m_pBlackboard->AddData("Ready", false);
}

bool DancingHumans::Update(float deltaTime)
{
	if (!Script::Update(deltaTime)) return false;

	bool readyToDance{false};
	m_pBlackboard->GetData("Ready", readyToDance);
	std::vector<NpcAgent*> dancers{ m_Roles[0].GetAgents() };
	
	if (!readyToDance)
	{
		bool allReady{ true };
		for (NpcAgent* pAgent : dancers)
		{
			if (pAgent == nullptr) continue;
			if (!pAgent->IsAtTarget()) allReady = false;
			pAgent->SetBodyColor({ 1,0,1,1 });
		}

		if (allReady) m_pBlackboard->ChangeData("Ready", true);	//If all are ready, continue
		else return true; //If not, agents are still moving into position
	}

	for (NpcAgent* pAudience : m_Roles[1].GetAgents())
	{
		pAudience->SetBodyColor({ 0,0,0,1 });
	}

	Elite::Vector2 center{};
	m_pBlackboard->GetData("location", center);
	for (NpcAgent* pAgent : dancers)
	{
		if (pAgent == nullptr) continue;

		Elite::Vector2 agentPos{ pAgent->GetPosition() };
		float currAngle{ Elite::GetOrientationFromVelocity({agentPos.x - center.x, agentPos.y - center.y}) };
		currAngle += deltaTime * m_CircleSpeed;
		pAgent->SetTarget(center + Elite::OrientationToVector(currAngle) * m_DanceCircleRadius);
		pAgent->SetBodyColor({ 0,1,1,1 });
	}

	return true;
}

bool DancingHumans::DynamicJoin()
{
	bool dynamicJoinSuccess = Script::DynamicJoin();
	if (!dynamicJoinSuccess) return false;

	std::vector<NpcAgent*> dynamicJoins{ m_Roles[1].GetAgents() };

	Elite::Vector2 center{};
	m_pBlackboard->GetData("location", center);

	for (NpcAgent* pAgent : dynamicJoins)
	{
		pAgent->SetToSeek();
		Elite::Vector2 agentPos{ pAgent->GetPosition() };
		Elite::Vector2 agentToOuterCircle{ agentPos.x - center.x, agentPos.y - center.y };
		Elite::Normalize(agentToOuterCircle);
		pAgent->SetTarget(center + agentToOuterCircle * m_WatchCircleRadius);
	}

	return true;
}
