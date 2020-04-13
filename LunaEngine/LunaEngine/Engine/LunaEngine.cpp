#include "stdafx.h"
#include "LunaEngine.h"

// Systems
#include "EntityManager.h"

LunaEngine* LunaEngine::m_Instance = nullptr;

LunaEngine::~LunaEngine()
{
}

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

void LunaEngine::Init()
{
}

bool LunaEngine::Update()
{
	return true;
}

void LunaEngine::Free()
{
}

LunaEngine::LunaEngine()
: m_EntityManager{ new EntityManager{ std::make_index_sequence<MAX_ENTITIES>{} } }
{
}
