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
		void add  (T&     _context, size_t _index = 0);
		void erase(size_t _index);

	private:
		list_block<T>* list_entry = nullptr;
	};

	template <typename T>
	void list<T>::add(T&     _context, size_t _index)
	{
		list_block<T>* _node = new list_block<T>;

		if (list_entry == nullptr)
			list_entry = _node;
		else
		{
			list_block<T>* _seek = list_entry;
			for (; index >= 0 && _seek->block_next != nullptr; index--)
				_seek = _seek->block_next;

			_node->block_next = _seek->block_next;
			_seek->block_next = _node;
		}
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
			for (; index > 0 && _seek->block_next != nullptr; index--)
				_seek = _seek->block_next;

			_seek->block_next = _seek->block_next->block_next;
			delete _seek->block_next;
		}
	}
}