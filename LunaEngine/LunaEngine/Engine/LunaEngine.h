#pragma once

// Must be forward declared to prevent circular header
class EntityManager;

// Singleton
class LunaEngine
{
public:

	// Disable copying
	LunaEngine(const LunaEngine&) = delete;
	LunaEngine& operator=(const LunaEngine&) = delete;

	~LunaEngine();

	static void CreateInstance();
	static void DeleteInstance();

	static LunaEngine* GetInstance();

	void Init();
	bool Update();
	void Free();

	// Systems

	
	// Managers
	EntityManager* m_EntityManager;

private:

	LunaEngine();

	static LunaEngine* m_Instance;
};