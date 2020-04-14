#pragma once
#include "FreeListAllocator.h"
#include <utility>
#include <memory>
#include <string>
#include <vector>

// This is currently about 1 MB per page
#define ALLOCATOR_PAGE_SIZE 1000000

class MemoryManager
{
public:

	static FreeListAllocator m_Allocator;

private:
};

// Used by STL Containers
template <typename T>
struct Allocator : public std::allocator<T>
{
	template <typename U>
	struct rebind
	{
		using others = Allocator<U>;
	};

	T* allocate(size_t n);

	void deallocate(T* p, size_t n);

	template <typename U>
	operator Allocator<U>();
};

template <typename T>
T* Allocator<T>::allocate(size_t n)
{
	return reinterpret_cast<T*>(MemoryManager::m_Allocator.Allocate(n));
}

template <typename T>
void Allocator<T>::deallocate(T* p, size_t n)
{
	UNREFERENCED_PARAMETER(n);
	MemoryManager::m_Allocator.Deallocate(p);
}

template <typename T>
template <typename U>
Allocator<T>::operator Allocator<U>()
{
	return Allocator<U>{};
}

// All STL containers using the allocator
namespace Luna
{

using string = std::basic_string<char, std::char_traits<char>, Allocator<char>>;

template <typename T>
using vector = std::vector<T, Allocator<T>>;

}