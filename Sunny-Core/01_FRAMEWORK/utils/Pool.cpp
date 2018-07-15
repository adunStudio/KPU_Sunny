#include "Pool.h"

namespace sunny
{
	namespace utils
	{
		// Pool
		Pool::Pool(PoolList* list)
		{
			prev = list->activePool->prev;          // 제일 앞에 붙인다. prev는 항상 ActivePool (이걸로 active임을 판단한다.)
			next = list->activePool;
			prev->next = next->prev = this;
		}

		Pool::~Pool()
		{
			prev->next = next;
			next->prev = prev;
		}

		// PoolList
		Pool* PoolList::GetPool(size_t index)
		{
			return (Pool*)(buffer + PoolSize * index);
		}

		PoolList::PoolList(size_t size, size_t count) : PoolSize(size), PoolCount(count)
		{
			buffer = new char[PoolSize * (PoolCount + 2)];     // 2개는 더미

			activePool = GetPool(0);                           // (0: 더미)
			activePool->prev = activePool->next = activePool;  // activePool 양 옆은 자기 자신

			freePool = GetPool(1);                             // (1: 더미)

			for (size_t i = 1; i < PoolCount + 2; ++i)        
			{
				GetPool(i)->prev = nullptr;
				GetPool(i)->next = GetPool(i + 1);
			}

			GetPool(PoolCount + 1)->next = freePool;           // (PoolCount + 2: 더미)
		}

		PoolList::~PoolList()
		{
			delete[] buffer;
		}

		void* PoolList::New(size_t size)
		{
			if (size > PoolSize)
			{
				// Debug System
				std::cout << "(PoolList::New) '" << size << " > " << PoolSize << std::endl;
				exit(1);
			}

			// 꽉 찼다면
			if (freePool->next == freePool) return nullptr;
		
			Pool* Pool = freePool->next;                     // free에서 하나 빼온다.
			freePool->next = Pool->next;

			return Pool;
		}

		void PoolList::Delete(void* p)
		{
			Pool* pool = (Pool*)p;

			if(pool->prev == nullptr)
			{
				// Debug System
				std::cout << "(PoolList::Delete) 잘못된 삭제" << std::endl;
				exit(1);
			}

			pool->prev = nullptr;
			pool->next = freePool->next;
			freePool->next = pool;
		}

		// PoolIter
		PoolIter::PoolIter(PoolList* list) : list(list), pool(list->activePool)
		{

		}

		bool PoolIter::HasNext()
		{
			return pool->next != list->activePool;
		}

		Pool* PoolIter::Next()
		{
			return pool = pool->next;
		}

		void PoolIter::Remove()
		{
			pool = pool->prev;

			delete pool->next;
		}
	}
}