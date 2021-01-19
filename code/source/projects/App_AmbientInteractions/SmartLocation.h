#pragma once
#include "projects/App_AmbientInteractions/NpcAgent.h"
#include "projects/App_AmbientInteractions/Script.h"

class SmartLocation
{
public:
	SmartLocation(const std::vector<Script*>& scripts, const Elite::Vector2& position, float notifyRange = 20.f);
	~SmartLocation();
	SmartLocation(const SmartLocation&) = delete;
	SmartLocation& operator=(const SmartLocation&) = delete;
	SmartLocation(SmartLocation&&) = delete;
	SmartLocation& operator=(SmartLocation&&) = delete;

	//Debug purposes only
	void Render() const;

	void NotifyAgents(std::vector<NpcAgent*>& agents);
	void StartScript();
	void UpdateScripts(float deltaTime);
private:

	Elite::Vector2 m_Position;
	float m_NotifyRange;

	std::vector<Script*> m_pScripts{};
	//Scripts should predefine amount of agents needed to start script
	std::vector<NpcAgent*> m_AvailableAgents{};
	Elite::Blackboard* m_pBlackboard = nullptr;

	Blackboard* MakeBlackboard();
};

