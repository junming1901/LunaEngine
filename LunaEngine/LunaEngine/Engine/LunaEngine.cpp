#include "stdafx.h"
#include "LunaEngine.h"

LunaEngine* LunaEngine::m_Instance = nullptr;

void LunaEngine::CreateInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new LunaEngine;
	}
}

void LunaEngine::DeleteInstance()
{
	delete m_Instance;
}

LunaEngine* LunaEngine::GetInstance()
{
	return m_Instance;
}

LunaEngine::LunaEngine()
{
}
