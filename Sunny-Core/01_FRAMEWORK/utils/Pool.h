#pragma once

#include "../include.h"

namespace sunny
{
	namespace utils
	{
		class Pool;
		class PoolList;
		class PoolIter;

		class Pool
		{
		private:
			friend PoolList;
			friend PoolIter;

		private:
			Pool* prev;
			Pool* next;

			void* operator new(size_t n) { };
			void operator delete(void* p) { };

		public:
			Pool(PoolList* list);
			virtual ~Pool();
		};

		class PoolList
		{
		private:
			friend Pool;
			friend PoolIter;

		private:
			char* buffer;

			Pool* activePool;
			Pool* freePool;

			size_t PoolSize;

			size_t PoolCount;

			Pool* GetPool(size_t index);

		public:
			int length;

			PoolList(size_t size, size_t count);
			~PoolList();

			void* New(size_t size);
			void Delete(void* p);
		};

		class PoolIter
		{
		private:
			PoolList* list;
			Pool* pool;

		public:
			PoolIter(PoolList* list);

			bool HasNext();

			Pool* Next();

			void Remove();
		};
	}
}