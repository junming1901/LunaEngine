#pragma once
#include "Entity.h"
#include "System.h"

#define MAX_ENTITIES 1000

class EntityManager : public System
{
public:

	template <typename T, T... ints>
	EntityManager(std::integer_sequence<T, ints...> seq);

private:

	Entity m_EntitiesList[MAX_ENTITIES];
};

// Initialize the entities with their ID at compile time
template <typename T, T ...ints>
inline EntityManager::EntityManager(std::integer_sequence<T, ints...> seq)
: m_EntitiesList { ints... }
{
	UNREFERENCED_PARAMETER(seq);
}
