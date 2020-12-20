#pragma once
#include "projects/Shared/SteeringAgent.h"

class NpcAgent final : public SteeringAgent
{
public:
	NpcAgent() : SteeringAgent() {};
	virtual ~NpcAgent() = default;

	bool IsAvailable() { return m_IsAvailable; }
	void Acquire() { m_IsAvailable = false; }
	void Release() { m_IsAvailable = true; }
private:
	bool m_IsAvailable = true;
};

