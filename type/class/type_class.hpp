#pragma once
#include <iostream>

namespace type
{
	template <typename T>
	struct class_property
	{
		static constexpr bool is_inherited = false;
	};

	template <class _Inherited, class _Parent>
	struct class_property<_Parent::_Inherited>
	{
		static constexpr bool is_inherited = true;
	};
}