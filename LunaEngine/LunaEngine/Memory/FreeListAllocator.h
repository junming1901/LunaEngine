#pragma once

struct FreeListAllocator
{
public:

	FreeListAllocator(size_t pagesize);

	void* Allocate(size_t size, int alignment = 8);
	void Deallocate(void* ptr);

private:

	struct PageHeader
	{
		PageHeader();

		PageHeader* m_Next;
	};

	struct FreeNode
	{
		FreeNode(size_t size, FreeNode* next = nullptr);

		size_t m_MemorySize;
		FreeNode* m_Next;
	};

	struct AllocationHeader
	{
		AllocationHeader(size_t blocksize, size_t padding);

		size_t m_BlockSize;
		size_t m_Padding;
	};

	// Memory Information
	size_t m_PageSize;
	int m_NumberPages;
	int m_FreeMemory;
	int m_MemoryUsed;
	int m_NumberAllocations;

	// Memory Data
	PageHeader* m_Pages;
	FreeNode* m_FreeNodes;

	void AllocateNewPage();

	// Return the prev node of the node inserted
	FreeNode* InsertFreeNode(FreeNode* node);

	void InsertFreeNode(FreeNode* node, FreeNode* prev);

	void RemoveFreeNode(FreeNode* node, FreeNode* prev);

	FreeNode* FindFreeBlock(size_t size, int alignment, int& padding, FreeNode*& prev);

	void MergeBlock(FreeNode* prev, FreeNode* curr);
};