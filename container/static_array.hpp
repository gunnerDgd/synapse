#include <iostream>
#include <variant>

namespace container
{
	template <typename T>
	class static_array
	{
	public:
		static_array  (size_t n)
			: static_context(new T[n]),
			  static_size	(n)  {}
		
		~static_array ()		 { delete[] static_context; }
		
		T& operator[] (size_t n) { return static_context[n % static_size]; }
		
	private:
		T	  *static_context;
		size_t static_size;
	};
}