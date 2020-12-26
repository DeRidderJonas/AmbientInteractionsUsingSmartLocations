#include "stdafx.h"
#include "Role.h"

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
