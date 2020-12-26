#pragma once
#include "projects/App_AmbientInteractions/Script.h"

class TwoAgentsTalking final : public Script
{
public:
	TwoAgentsTalking();
	virtual ~TwoAgentsTalking() = default;

	virtual bool IsPreconditionMet(Elite::Blackboard* pBlackboard) override;
	virtual bool IsEndConditionMet() override;
	virtual void Start(Elite::Blackboard* pBlackboard) override;
	virtual bool Update(float deltaTime) override;
	virtual void End() override;
private:
	float m_AmountOfSecondsTalking{ 10.f };
};

