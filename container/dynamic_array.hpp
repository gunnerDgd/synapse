#include <iostream>

namespace memory
{
	template <typename T>
	class dynamic_array
	{
	public:
		dynamic_array(size_t _init_size)
			: dynamic_context(new T[_init_size]),
			  dynamic_size   (_init_size) {}
		
		void add  ();
		void erase();
		
	private:
		T*	   dynamic_context;
		size_t dynamic_size; 
	}
}