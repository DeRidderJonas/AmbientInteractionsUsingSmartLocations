#include "stdafx.h"
#include "Script.h"
#include <projects/App_AmbientInteractions/NpcAgent.h>

Script::~Script()
{
	if(m_pBlackboard)
		delete m_pBlackboard;
}

bool Script::IsPreconditionMet(Elite::Blackboard* pBlackboard)
{
	return !m_IsRunning && m_CooldownRemaining <= 0.f;
}

bool Script::IsEndConditionMet()
{
	return m_IsRunning;
}

void Script::Start(Elite::Blackboard* pBlackboard)
{
	m_pBlackboard = pBlackboard;
	m_IsRunning = true;
	m_TimeElapsed = 0.f;
	m_CooldownRemaining = m_Cooldown;
}

bool Script::Update(float deltaTime)
{
	if (m_IsRunning) m_TimeElapsed += deltaTime;
	else m_CooldownRemaining -= deltaTime;

	return m_IsRunning;
}

void Script::End()
{
	for (Role& role : m_Roles) role.OnScriptEnd();

	delete m_pBlackboard;
	m_pBlackboard = nullptr;
	m_IsRunning = false;
}

void Script::OnError()
{
	End();
}

bool Script::AreAllRolesMet(Elite::Blackboard* pBlackboard)
{
	std::vector<NpcAgent*>* pAgents = nullptr;
	bool dataAvailable = pBlackboard->GetData("agents", pAgents);
	if (!dataAvailable) return false;

	for (NpcAgent* pAgent : *pAgents)
	{
		for (Role& role : m_Roles)
		{
			if (pAgent->CanAssumeRole(role.GetNames()))
			{
				if (role.IsCardinalitySatisfied()) //Minimum amount of agents has been reached
				{
					continue;
				}

				role.IncrementCardinality();
			}

		}
	}

	bool isValid = true;
	for (Role& role : m_Roles)
	{
		if (!role.IsCardinalitySatisfied()) isValid = false;
		role.ResetCardinality();
	}

	return isValid;
}

bool Script::RoleAllocation()
{
	std::vector<NpcAgent*>* pAgents = nullptr;
	bool dataAvailable = m_pBlackboard->GetData("agents", pAgents);
	if (!dataAvailable) return false;

	//Shuffle all participants to counter any unintentional bias towards first or last NPC
	std::random_shuffle(pAgents->begin(), pAgents->end());

	std::map<Role*, NpcAgent*> agentsReserve{};
	for (NpcAgent* pAgent : *pAgents)
	{
		for (Role& role : m_Roles)
		{
			if (role.IsDynamicJoin()) continue;
			if (pAgent->CanAssumeRole(role.GetNames()))
			{
				if (role.IsCardinalitySatisfied()) //Minimum amount of agents has been reached
				{
					agentsReserve.emplace(&role, pAgent);
					continue;
				}

				role.IncrementCardinality();
				role.AcquireAgent(pAgent);
			}
		}
	}

	//Fill up slots to max cardinality if possible
	for (const std::pair<Role*, NpcAgent*>& pair : agentsReserve)
	{
		//If claimed by role in previous loop or previous role in this loop, ignore agent
		if (!pair.second->IsAvailable() || !pair.first->CanAcquireAgent()) continue;

		pair.first->AcquireAgent(pair.second);
	}

	for (Role& role : m_Roles)
	{
		if (role.IsDynamicJoin()) continue;
		if (!role.IsCardinalitySatisfied()) return false;
	}

	return true;
}
