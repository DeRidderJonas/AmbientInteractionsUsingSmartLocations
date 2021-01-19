#include "stdafx.h"
#include "App_AmbientInteractions.h"
#include "Scripts/DancingHumans.h"
#include "Scripts/AgentsTalking.h"

App_AmbientInteractions::~App_AmbientInteractions()
{
	for (NpcAgent* pAgent : m_pAgents) delete pAgent;
	for (SmartLocation* pSL : m_pSmartLocations) delete pSL;
}

void App_AmbientInteractions::Start()
{
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(20.f);

	for (int i = 0; i < m_AmountOfAgents; i++)
	{
		NpcAgent* newAgent{ new NpcAgent(Role::Name::AnyHuman) };
		newAgent->SetBodyColor({ 1,0,0 });
		newAgent->SetMass(0.f);
		m_pAgents.push_back(newAgent);
		m_pAgents[i]->SetPosition(Elite::randomVector2(50.f));
	}
	

	std::vector<Script*> sl0{ new AgentsTalking(2, 1) };
	m_pSmartLocations.push_back(new SmartLocation(sl0, Elite::Vector2{-m_TrimWorldSize / 2.f,0}));

	std::vector<Script*> sl1{ new DancingHumans(3, 4, 1, 5) };
	m_pSmartLocations.push_back(new SmartLocation(sl1, Elite::Vector2{ m_TrimWorldSize / 2.f, 0.f }));
}

void App_AmbientInteractions::Update(float deltaTime)
{
	for (NpcAgent* pAgent : m_pAgents)
	{
		pAgent->SetBodyColor({ 1,0,0 });
		pAgent->Update(deltaTime);
		pAgent->TrimToWorld(m_TrimWorldSize);
	}

	for (SmartLocation* sl : m_pSmartLocations)
	{
		sl->NotifyAgents(m_pAgents);
		//If StartScript not in the same loop body as notifyAgents, 2 scripts might try to claim the same Agent
		sl->StartScript();
		sl->UpdateScripts(deltaTime);
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

	for (const SmartLocation* sl : m_pSmartLocations)
	{
		sl->Render();
	}

	for (NpcAgent* pAgent : m_pAgents)
	{
		pAgent->Render(deltaTime);
	}
}
