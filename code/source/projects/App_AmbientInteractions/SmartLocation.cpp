#include "stdafx.h"
#include "SmartLocation.h"

SmartLocation::SmartLocation(const Elite::Vector2& position, float notifyRange)
	: m_Position{ position }
	, m_NotifyRange{notifyRange}
{
}

void SmartLocation::Render() const
{
	DEBUGRENDERER2D->DrawCircle(m_Position, m_NotifyRange, { 0,1,0,1 }, 0.7f);
}

void SmartLocation::NotifyAgents(std::vector<SteeringAgent*>& agents)
{
	m_AvailableAgents.clear();

	const float notifyRangeSqrd{ m_NotifyRange * m_NotifyRange };
	for (SteeringAgent* pAgent : agents)
	{
		if (Elite::DistanceSquared(pAgent->GetPosition(), m_Position) <= notifyRangeSqrd)
		{
			m_AvailableAgents.push_back(pAgent);
			pAgent->SetBodyColor({ 0,1,0,1 });
		}
	}
}
