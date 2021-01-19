#pragma once
#include "projects/App_AmbientInteractions/Script.h"

class DancingHumans : public Script
{
public:
	DancingHumans(size_t min_agents, size_t max_agents, size_t min_agents_dynamic, size_t max_agents_dynamic);
	DancingHumans(size_t agents, size_t agents_dynamic);
	virtual ~DancingHumans() = default;

	virtual bool IsEndConditionMet() override;
	virtual void Start(Elite::Blackboard* pBlackboard) override;
	virtual bool Update(float deltaTime) override;
	virtual bool DynamicJoin() override;
private:
	float m_AmountOfSecondsToDance{ 30.f };
	float m_DanceCircleRadius{ 10.f };
	float m_WatchCircleRadius{ 15.f };
	float m_CircleSpeed{ 5.f };
};

