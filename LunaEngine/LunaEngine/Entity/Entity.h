#pragma once
#include "MemoryManager.h"

class Entity
{
public:

	Entity(int index);

private:

	const int m_Index;

	Luna::string m_Name;
};