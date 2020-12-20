#pragma once
#include "projects/Shared/SteeringAgent.h"

class SmartLocation
{
public:
	SmartLocation(const Elite::Vector2& position, float notifyRange = 10.f);
	~SmartLocation() = default;

	//Debug purposes only
	void Render() const;

	void NotifyAgents(std::vector<SteeringAgent*>& agents);
private:

	Elite::Vector2 m_Position;
	float m_NotifyRange;


	//Scripts should predefine amount of agents needed to start script
	std::vector<SteeringAgent*> m_AvailableAgents{};
};

