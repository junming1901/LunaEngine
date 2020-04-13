#pragma once
#include "FreeListAllocator.h"

void* operator new(size_t count);
void operator delete(void* ptr);

class MemoryManager
{
public:

	static FreeListAllocator m_Allocator;

private:
};

template <typename T>
class Allocator
{
public:



private:
};