#pragma once
#include "framework/EliteInterfaces/EIApp.h"
#include "projects/App_AmbientInteractions/NpcAgent.h"
#include "SmartLocation.h"

class App_AmbientInteractions final : public IApp
{
public:

	App_AmbientInteractions() = default;
	virtual ~App_AmbientInteractions();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(float deltaTime) const override;

private:
	//Agents
	const int m_AmountOfAgents{ 20 };
	std::vector<NpcAgent*> m_pAgents{};

	//World
	const float m_TrimWorldSize{ 70.f };

	//Smart locations
	std::vector<SmartLocation*> m_pSmartLocations{};
};

