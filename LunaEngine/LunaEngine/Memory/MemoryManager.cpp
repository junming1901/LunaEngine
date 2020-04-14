#include "stdafx.h"
#include "MemoryManager.h"

FreeListAllocator MemoryManager::m_Allocator = FreeListAllocator{ ALLOCATOR_PAGE_SIZE };