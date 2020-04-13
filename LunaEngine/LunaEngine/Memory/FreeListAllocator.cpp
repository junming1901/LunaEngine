#include "stdafx.h"
#include "FreeListAllocator.h"
#include <stdlib.h>
#include <iostream>
#include <new>

#define DEBUG_FREELIST true

FreeListAllocator::FreeListAllocator(size_t pagesize)
: m_PageSize { pagesize }, m_NumberPages { 0 }, m_Pages { nullptr },  m_FreeNodes { nullptr }, 
  m_FreeMemory { 0 }, m_MemoryUsed { 0 }, m_NumberAllocations { 0 }
{
	AllocateNewPage();
}

void* FreeListAllocator::Allocate(size_t size, int alignment)
{
	int padding = 0;

	FreeNode* prev = nullptr;

	// Find a free block for the memory
	FreeNode* node = FindFreeBlock(size, alignment, padding, prev);

	RemoveFreeNode(node, prev);

	const int AlignmentPadding = padding - sizeof(AllocationHeader);
	int TotalSize = size + padding;
	const size_t LeftoverSpace = node->m_MemorySize - static_cast<size_t>(TotalSize);

	// Check if there is enough leftover space to insert as another free node
	if (LeftoverSpace > sizeof(FreeNode))
	{
		FreeNode* freenode = reinterpret_cast<FreeNode*>(reinterpret_cast<char*>(node) + TotalSize);

		new (freenode) FreeNode{ LeftoverSpace };

		InsertFreeNode(freenode, prev);
	}
	else
	{
		// Add it to the total size so that the data will not be wasted
		TotalSize += LeftoverSpace;
	}

	AllocationHeader* AllocatedHeader = reinterpret_cast<AllocationHeader*>(reinterpret_cast<char*>(node) + AlignmentPadding);
	void* Data = reinterpret_cast<void*>(reinterpret_cast<char*>(AllocatedHeader) + sizeof(AllocatedHeader));

	AllocatedHeader->m_BlockSize = TotalSize;
	AllocatedHeader->m_Padding = AlignmentPadding;

	m_MemoryUsed += TotalSize;
	m_FreeMemory -= TotalSize;

	++m_NumberAllocations;

#if DEBUG_FREELIST
	std::cout << "Allocate - Node Address Start: " << node << std::endl;
	std::cout << "Allocate - Node Address End: " << reinterpret_cast<FreeNode*>(reinterpret_cast<char*>(node) + TotalSize) << std::endl;
	std::cout << "Allocate - Padding: " << AlignmentPadding << std::endl;
	std::cout << "Allocate - Allocated Head Address: " << AllocatedHeader << std::endl;
	std::cout << "Allocate - Block Size: " << TotalSize << std::endl;
	std::cout << "Allocate - Data Address: " << Data << std::endl << std::endl;
#endif

	return Data;
}

void FreeListAllocator::Deallocate(void* ptr)
{
	AllocationHeader* AllocatedHeader = reinterpret_cast<AllocationHeader*>(reinterpret_cast<char*>(ptr) - sizeof(AllocationHeader));

	FreeNode* node = reinterpret_cast<FreeNode*>(reinterpret_cast<char*>(AllocatedHeader) - AllocatedHeader->m_Padding);

#if DEBUG_FREELIST
	std::cout << "Deallocate - Node Address: " << node << std::endl;
	std::cout << "Deallocate - Allocated Header Address: " << AllocatedHeader << std::endl;
	std::cout << "Deallocate - Padding: " << AllocatedHeader->m_Padding << std::endl;
	std::cout << "Deallocate - Block Size: " << AllocatedHeader->m_BlockSize << std::endl;
	std::cout << "Deallocate - Data Addresses: " << ptr << std::endl << std::endl;
#endif

	m_FreeMemory += AllocatedHeader->m_BlockSize;
	m_MemoryUsed -= AllocatedHeader->m_BlockSize;

	--m_NumberAllocations;

	new (node) FreeNode{ AllocatedHeader->m_BlockSize };

	FreeNode* prev = InsertFreeNode(node);

	MergeBlock(prev, node);
}

void FreeListAllocator::AllocateNewPage()
{
	char* new_memory = reinterpret_cast<char*>(malloc(m_PageSize));

	new (new_memory) PageHeader;

	// Add the page to the page pointer
	if (m_Pages == nullptr)
	{
		m_Pages = reinterpret_cast<PageHeader*>(new_memory);
	}
	else
	{
		PageHeader* pages = m_Pages;

		while (pages->m_Next != nullptr)
		{
			pages = pages->m_Next;
		}

		pages->m_Next = reinterpret_cast<PageHeader*>(new_memory);
	}

	// Add the page to the free nodes
	FreeNode* freenode = reinterpret_cast<FreeNode*>(m_Pages + sizeof(PageHeader));

	new (freenode) FreeNode{ m_PageSize - sizeof(PageHeader) };

	InsertFreeNode(freenode);

	++m_NumberPages;
}

FreeListAllocator::FreeNode* FreeListAllocator::InsertFreeNode(FreeNode* node)
{
	m_FreeMemory += node->m_MemorySize;

	// Check if there are any free nodes
	if (m_FreeNodes == nullptr)
	{
		m_FreeNodes = node;
		return nullptr;
	}
	// Check if the address is before the first free node
	else if (node < m_FreeNodes)
	{
		node->m_Next = m_FreeNodes;
		m_FreeNodes = node;
		return nullptr;
	}

	FreeNode* current = m_FreeNodes;

	// Either go to the node whose next node address is more than the node to insert 
	// or the last node
	while (current->m_Next != nullptr && current->m_Next < node)
	{
		current = current->m_Next;
	}

	node->m_Next = current->m_Next;
	current->m_Next = node;

	return current;
}

void FreeListAllocator::InsertFreeNode(FreeNode* node, FreeNode* prev)
{
	if (prev == nullptr)
	{
		m_FreeNodes = node;
		return;
	}

	node->m_Next = prev->m_Next;
	prev->m_Next = node;
}

void FreeListAllocator::RemoveFreeNode(FreeNode* node, FreeNode* prev)
{
	if (prev == nullptr)
	{
		m_FreeNodes = node->m_Next;
	}
	else
	{
		prev->m_Next = node->m_Next;
	}
}

FreeListAllocator::FreeNode* FreeListAllocator::FindFreeBlock(size_t size, int alignment, int& padding, FreeNode*& prev)
{
	int MinimumSpace = (sizeof(AllocationHeader) / alignment) * alignment;

	// Check if the allocation header is a multiple of the alignent
	if (sizeof(AllocationHeader) % alignment != 0)
	{
		MinimumSpace += alignment;
	}

	int SizeAlignment = size % alignment;

	// Calculate how much padding is needed at the end
	if (SizeAlignment != 0)
	{
		SizeAlignment = alignment - SizeAlignment;
	}

	FreeNode* prev_node = nullptr;
	FreeNode* node = m_FreeNodes;

	while (node != nullptr)
	{
		const int StartAddress = reinterpret_cast<int>(node);

		// Calculate the number of padding needed
		padding = StartAddress % alignment + SizeAlignment;

		// Check if the padding is enough to accomodate the minimum space required
		if (padding < MinimumSpace)
		{
			int remainder = MinimumSpace - padding;
			bool notmultipler = remainder % alignment;

			padding += (remainder / alignment + notmultipler) * alignment;
		}

		// Check if the current block is big enough to hold the padding and the size
		if (node->m_MemorySize >= size + padding)
		{
			prev = prev_node;
			return node;
		}

		prev_node = node;
		node = node->m_Next;
	}

	// This means that no block could be found to fit
	AllocateNewPage();

	return FindFreeBlock(size, alignment, padding, prev);
}

void FreeListAllocator::MergeBlock(FreeNode* prev, FreeNode* curr)
{
	// Check if need to merge the MergeNode with the Next
	if (curr->m_Next != nullptr &&
		reinterpret_cast<FreeNode*>(reinterpret_cast<char*>(curr) + curr->m_MemorySize) == curr->m_Next)
	{
		curr->m_MemorySize += curr->m_Next->m_MemorySize;

#if DEBUG_FREELIST
		std::cout << "Deallocate - Merging Current: " << curr << " with Next: " << curr->m_Next << std::endl;
#endif

		RemoveFreeNode(curr, curr->m_Next);
	}

	// Check if need to merge the Prev witht he MergeNode
	if (prev != nullptr &&
		reinterpret_cast<FreeNode*>(reinterpret_cast<char*>(prev) + prev->m_MemorySize) == curr)
	{
		prev->m_MemorySize += curr->m_MemorySize;

#if DEBUG_FREELIST
		std::cout << "Deallocate - Merging Previous: " << prev << " with Current: " << curr << std::endl;
#endif

		RemoveFreeNode(prev, curr);
	}
}

FreeListAllocator::PageHeader::PageHeader()
: m_Next { nullptr }
{
}

FreeListAllocator::FreeNode::FreeNode(size_t size, FreeNode* next)
: m_MemorySize { size }, m_Next { next }
{
}

FreeListAllocator::AllocationHeader::AllocationHeader(size_t blocksize, size_t padding)
: m_BlockSize { blocksize }, m_Padding { padding }
{
}
