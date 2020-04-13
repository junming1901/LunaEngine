#pragma once

struct System
{
public:

	System(const System&) = delete;
	System& operator=(const System&) = delete;

	friend class LunaEngine;

protected:

	System() = default;
};