#include "stdafx.h"
#include "MemoryManager.h"

FreeListAllocator MemoryManager::m_Allocator = FreeListAllocator{ ALLOCATOR_PAGE_SIZE };

void* operator new(size_t count)
{
	return MemoryManager::m_Allocator.Allocate(count);
}

void operator delete(void* ptr)
{
	return MemoryManager::m_Allocator.Deallocate(ptr);
}
