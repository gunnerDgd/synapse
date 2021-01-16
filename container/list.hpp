#pragma once
#include <iostream>

namespace container
{
	template <typename T>
	struct list_block
	{
		T			   block_context;
		list_block<T>* block_next = nullptr;
	};

	template <typename T>
	class list
	{
	public:
		void add  (T      _context, size_t _index = 0);
		void erase(size_t _index);

	private:
		list_block<T>* list_entry = nullptr;
		size_t		   list_count = 0;
	};

	template <typename T>
	void list<T>::add(T      _context, size_t _index)
	{
		list_block<T>* _node = new list_block<T>;
		_node->block_context = _context;

		if (list_entry == nullptr)
			list_entry = _node;
		else
		{
			list_block<T>* _seek = list_entry;
			for (; _index >= 0 && _seek->block_next != nullptr; _index--)
				_seek = _seek->block_next;

			_node->block_next = _seek->block_next;
			_seek->block_next = _node;
		}
		
		++list_count;
	}

	template <typename T>
	void list<T>::erase(size_t _index)
	{
		list_block<T>* _seek = list_entry;
		if (_index == 0)
		{
			list_entry = _seek->block_next;
			delete _seek;
		}
		else
		{
			for (; _index > 0 && _seek->block_next != nullptr; _index--)
				_seek = _seek->block_next;

			_seek->block_next = _seek->block_next->block_next;
			delete _seek->block_next;
		}
		
		--list_count;
	}
}