#include "stdafx.h"
#include "Script.h"

Script::~Script()
{
	if(m_pBlackboard)
		delete m_pBlackboard;
}

void Script::Start(Elite::Blackboard* pBlackboard)
{
	m_pBlackboard = pBlackboard;
	m_IsRunning = true;
	m_TimeElapsed = 0.f;
}

bool Script::Update(float deltaTime)
{
	if (m_IsRunning) m_TimeElapsed += deltaTime;
	return m_IsRunning;
}

void Script::End()
{
	delete m_pBlackboard;
	m_pBlackboard = nullptr;
	m_IsRunning = false;
}

void Script::OnError()
{
	End();
}
