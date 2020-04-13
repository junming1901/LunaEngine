#pragma once
#include <string>

class Entity
{
public:

	Entity(int index);

private:

	const int m_Index;

	std::string m_Name;
};