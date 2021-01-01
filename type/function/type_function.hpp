#include <iostream>
#include <synapse/synapse.hpp>

namespace type
{
	template <typename T>
	class function_property
	{
		static constexpr bool   is_method   = false;
		static constexpr size_t param_count = 0;
	};

    template <typename R, typename... P, typename C>
    class function_property<R(C::*)(P...)>
    {
        public:
            static constexpr bool   is_method   = true;
            static constexpr size_t param_count = sizeof...(P);
    };

    template <typename R, typename... P>
    class function_property<R(*)(P...)>
    {
        public:
            static constexpr bool   is_method   = false;
            static constexpr size_t param_count = sizeof...(P);
    };

#if CPPVER >= 17
    template <typename F>
    concept bool is_method = function_property<F>::is_method;
#endif
}