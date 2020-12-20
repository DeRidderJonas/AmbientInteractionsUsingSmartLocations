#include "stdafx.h"
#include "App_AmbientInteractions.h"

App_AmbientInteractions::~App_AmbientInteractions()
{
	for (SteeringAgent* pAgent : m_pAgents) delete pAgent;
}

void App_AmbientInteractions::Start()
{
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(20.f);

	for (int i = 0; i < m_AmountOfAgents; i++)
	{
		SteeringAgent* newAgent{ new SteeringAgent() };
		newAgent->SetBodyColor({ 1,0,0 });
		m_pAgents.push_back(newAgent);
		m_pAgents[i]->SetPosition(Elite::randomVector2(50.f));
	}

	m_SmartLocations.push_back(SmartLocation{ Elite::Vector2{0,0} });
}

void App_AmbientInteractions::Update(float deltaTime)
{
	for (SteeringAgent* pAgent : m_pAgents)
	{
		pAgent->SetBodyColor({ 1,0,0 });
		pAgent->Update(deltaTime);
		pAgent->TrimToWorld(m_TrimWorldSize);
	}

	for (SmartLocation& sl : m_SmartLocations)
	{
		sl.NotifyAgents(m_pAgents);
	}
}

void App_AmbientInteractions::Render(float deltaTime) const
{
	std::vector<Elite::Vector2> points =
	{
		{ -m_TrimWorldSize, m_TrimWorldSize },
		{ m_TrimWorldSize, m_TrimWorldSize },
		{ m_TrimWorldSize, -m_TrimWorldSize },
		{ -m_TrimWorldSize, -m_TrimWorldSize }
	};
	DEBUGRENDERER2D->DrawPolygon(&points[0], 4, { 1,0,0,1 }, 0.4f);

	for (const SmartLocation& sl : m_SmartLocations)
	{
		sl.Render();
	}

	for (SteeringAgent* pAgent : m_pAgents)
	{
		pAgent->Render(deltaTime);
	}
}
