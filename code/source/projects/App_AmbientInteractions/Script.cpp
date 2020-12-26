#include "stdafx.h"
#include "Script.h"
#include <projects/App_AmbientInteractions/NpcAgent.h>

Script::~Script()
{
	if(m_pBlackboard)
		delete m_pBlackboard;
}

void Script::Start(Elite::Blackboard* pBlackboard)
{
	m_pBlackboard = pBlackboard;
	m_IsRunning = true;
	m_TimeElapsed = 0.f;
}

bool Script::Update(float deltaTime)
{
	if (m_IsRunning) m_TimeElapsed += deltaTime;
	return m_IsRunning;
}

void Script::End()
{
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
