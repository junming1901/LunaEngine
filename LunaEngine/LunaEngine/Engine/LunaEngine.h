#pragma once

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
	void Update();
	void Free();

	// Systems

	
	// Managers


private:

	LunaEngine();

	static LunaEngine* m_Instance;
};