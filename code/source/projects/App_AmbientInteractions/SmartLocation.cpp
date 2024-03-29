#include "stdafx.h"
#include "SmartLocation.h"
#include "projects/App_AmbientInteractions/Scripts/AgentsTalking.h"
#include "projects/App_AmbientInteractions/Scripts/DancingHumans.h"

SmartLocation::SmartLocation(const std::vector<Script*>& scripts, const Elite::Vector2& position, float notifyRange)
	: m_pScripts{ scripts }
	, m_Position{ position }
	, m_NotifyRange{notifyRange}
{
	//Setup smart location blackboard
	m_pBlackboard = new Elite::Blackboard();
	m_pBlackboard->AddData("agents", &m_AvailableAgents);
}

SmartLocation::~SmartLocation()
{
	for (Script* pScript : m_pScripts) delete pScript;
	SAFE_DELETE(m_pBlackboard);
}

void SmartLocation::Render() const
{
	DEBUGRENDERER2D->DrawCircle(m_Position, m_NotifyRange, { 0,1,0,1 }, 0.7f);
}

void SmartLocation::NotifyAgents(std::vector<NpcAgent*>& agents)
{
	m_AvailableAgents.clear();

	const float notifyRangeSqrd{ m_NotifyRange * m_NotifyRange };
	for (NpcAgent* pAgent : agents)
	{
		if (pAgent->IsAvailable() && Elite::DistanceSquared(pAgent->GetPosition(), m_Position) <= notifyRangeSqrd)
		{
			m_AvailableAgents.push_back(pAgent);
			pAgent->SetBodyColor({ 0,1,0,1 });
		}
	}
}

void SmartLocation::StartScript()
{
	std::vector<Script*> availableScripts{};
	for (Script* pScript : m_pScripts)
	{
		if (pScript->IsPreconditionMet(m_pBlackboard)) availableScripts.push_back(pScript);
	}
	if (availableScripts.size() == 0) return;

	Elite::Blackboard* pBlackboard{ MakeBlackboard() };
	size_t randScript{ rand() % availableScripts.size() };
	availableScripts[randScript]->Start(pBlackboard);
}

void SmartLocation::UpdateScripts(float deltaTime)
{
	for (Script* pScript : m_pScripts)
	{
		bool isActive = pScript->Update(deltaTime);
		if (!isActive) continue;

		if (pScript->IsDynamicJoinPrecoditionMet())
		{
			pScript->DynamicJoin();
		}

		if (pScript->IsEndConditionMet())
		{
			pScript->End();
		}
	}
}

Blackboard* SmartLocation::MakeBlackboard()
{
	Elite::Blackboard* pBlackboard = new Elite::Blackboard();
	pBlackboard->AddData("agents", &m_AvailableAgents);
	pBlackboard->AddData("location", m_Position);

	return pBlackboard;
}
