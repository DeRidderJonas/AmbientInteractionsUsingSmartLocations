#include "stdafx.h"
#include "Role.h"
#include "NpcAgent.h"

Role::Role(Name names, Flag flags, size_t cardinality_min, size_t cardinality_max)
	: m_Names{names}
	, m_Flags{flags}
	, m_Cardinality_min{cardinality_min}
	, m_Cardinality_max{cardinality_max}
{
}

Role::Role(Name names, Flag flags, size_t cardinality)
	: Role(names, flags, cardinality, cardinality)
{
}

Role::Name Role::GetNames() const
{
	return m_Names;
}

bool Role::IsDynamicJoin() const
{
	return int(m_Flags & Flag::DynamicJoin) > 0;
}

void Role::IncrementCardinality()
{
	m_AmountNPCs++;
}

void Role::ResetCardinality()
{
	m_AmountNPCs = 0;
}

bool Role::IsCardinalitySatisfied() const
{
	return m_AmountNPCs >= m_Cardinality_min && m_AmountNPCs <= m_Cardinality_max;
}

bool Role::CanAcquireAgent() const
{
	return m_pAgents.size() < m_Cardinality_max;
}

void Role::AcquireAgent(NpcAgent* pAgent)
{
	pAgent->Acquire();
	m_pAgents.push_back(pAgent);
}

std::vector<NpcAgent*>& Role::GetAgents()
{
	return m_pAgents;
}

void Role::OnScriptEnd()
{
	for (NpcAgent* pAgent : m_pAgents)
	{
		pAgent->Release();
		if (int(m_Flags & Flag::LeaveAfterEnd) > 0) pAgent->SetToWander();
	}

	ResetCardinality();
	m_pAgents.clear();
}
