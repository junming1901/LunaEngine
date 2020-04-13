#pragma once
#include "FreeListAllocator.h"
#include <utility>
#include <memory>

// This is currently about 1 MB per page
#define ALLOCATOR_PAGE_SIZE 1000000

// Overloading global new and delete
// Also called by std containers
void* operator new(size_t count);
void* operator new[](size_t count);

void operator delete(void* ptr);
void operator delete[](void* ptr);

class MemoryManager
{
public:

	static FreeListAllocator m_Allocator;

private:
};