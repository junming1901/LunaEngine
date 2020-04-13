#pragma once
#include "FreeListAllocator.h"
#include <utility>
#include <memory>

#define ALLOCATOR_PAGE_SIZE 1000000

void* operator new(size_t count);
void operator delete(void* ptr);

class MemoryManager
{
public:

	static FreeListAllocator m_Allocator;

private:
};


// To be used for all std containers
template <typename T>
class Allocator : public std::allocator<T>
{
public:

	template <typename U>
	struct rebind
	{
		using other = Allocator<U>;
	};

	T* allocate(size_t n);
	void deallocate(T* ptr, size_t n);

	size_t max_size() const;

	template <typename ... args>
	void construct(T* ptr, args&&... arg);

	void destroy(T* ptr);

	template <typename U>
	operator Allocator<U>();

private:
};

template <typename T>
inline T* Allocator<T>::allocate(size_t n)
{
	return reinterpret_cast<T*>(MemoryManager::m_Allocator.Allocate(n * sizeof(T)));
}

template <typename T>
inline void Allocator<T>::deallocate(T* ptr, size_t n)
{
	UNREFERENCED_PARAMETER(n);

	MemoryManager::m_Allocator.Deallocate(ptr);
}

template <typename T>
inline size_t Allocator<T>::max_size() const
{
	return ALLOCATOR_PAGE_SIZE;
}

template <typename T>
inline void Allocator<T>::destroy(T* ptr)
{
	ptr->~T();
}

template <typename T>
template <typename ... args>
inline void Allocator<T>::construct(T* ptr, args&&... arg)
{
	new (ptr) T{ std::forward<args...>(arg...) };
}

template <typename T>
template <typename U>
inline Allocator<T>::operator Allocator<U>()
{
	return Allocator<U>{};
}
