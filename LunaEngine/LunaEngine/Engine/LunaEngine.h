#pragma once

// Singleton
class LunaEngine
{
public:

	// Disable copying
	LunaEngine(const LunaEngine&) = delete;
	LunaEngine& operator=(const LunaEngine&) = delete;

	static void CreateInstance();
	static void DeleteInstance();

	static LunaEngine* GetInstance();

	// Systems

	// Managers


private:

	LunaEngine();

	static LunaEngine* m_Instance;
};