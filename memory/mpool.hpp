#pragma once
#include <iostream>
#include <atomic>

namespace memory
{
	template <class T>
	class memory_block
	{
	public:
		T& get() { return block_context; }

		template <class T, size_t N>
		friend class memory_pool;

	private:
		T				 block_context;
		memory_block<T>* block_next = nullptr;
	};

	template <class T, size_t N>
	class memory_pool
	{
	public:
		memory_pool()
		{
			mblock_entry = new memory_block<T>[N];
			for (size_t i = 0; i < N - 1; i++)
				mblock_entry[i].block_next = (mblock_entry.load() + i + 1);
		}

		memory_block<T>*			  acquire();
		void						  release(memory_block<T>* _res);

	private:
		std::atomic<memory_block<T>*> mblock_entry = nullptr;
	};

	template <class T, size_t N>
	memory_block<T>* memory_pool<T, N>::acquire() 
	{
		memory_block<T>* _chk;
		do
		{
			_chk 			   = mblock_entry.load(std::memory_order_relaxed);
			if (_chk == nullptr) return nullptr;

		} while (!mblock_entry.compare_exchange_weak(_chk, _chk->block_next,
													 std::memory_order_release,
													 std::memory_order_relaxed));
		return _chk;
	}

	template <class T, size_t N>
	void			 memory_pool<T, N>::release(memory_block<T>* _res)
	{
		do
		{
			_res->block_next = mblock_entry.load(std::memory_order_relaxed);

		} while (!mblock_entry.compare_exchange_weak(_res->block_next, _res,
													 std::memory_order_release,
													 std::memory_order_relaxed));
	}
}