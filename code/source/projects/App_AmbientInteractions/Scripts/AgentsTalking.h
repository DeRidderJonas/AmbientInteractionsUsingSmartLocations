#pragma once
#include "projects/App_AmbientInteractions/Script.h"

class AgentsTalking final : public Script
{
public:
	AgentsTalking(size_t min_agents, size_t max_agents, size_t min_agents_dynamic, size_t max_agents_dynamic);
	AgentsTalking(size_t agents, size_t agents_dynamic);
	virtual ~AgentsTalking() = default;

	virtual bool IsEndConditionMet() override;
	virtual void Start(Elite::Blackboard* pBlackboard) override;
	virtual bool Update(float deltaTime) override;
	virtual void End() override;
	virtual bool DynamicJoin() override;
private:
	float m_AmountOfSecondsTalking{ 10.f };
};

