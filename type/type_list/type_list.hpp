#pragma once
#include <iostream>

namespace type
{
	enum class type_id
	{
		t_char,
		t_uchar,

		t_short,
		t_ushort,

		t_int,
		t_uint,

		t_int64,
		t_uint64,

		t_float,
		t_double,

		t_other
	};

	template <typename T>
	struct type_block 
	{
		static constexpr type_id id = type_id::t_other;
	};

	template <>
	struct type_block<unsigned char>
	{
		static constexpr type_id id = type_id::t_uchar;
	};

	template <>
	struct type_block<unsigned short>
	{
		static constexpr type_id id = type_id::t_ushort;
	};

	template <>
	struct type_block<unsigned int>
	{
		static constexpr type_id id = type_id::t_uint;
	};

	template <>
	struct type_block<uint64_t>
	{
		static constexpr type_id id = type_id::t_uint64;
	};

	template <>
	struct type_block<char>
	{
		static constexpr type_id id = type_id::t_char;
	};

	template <>
	struct type_block<short>
	{
		static constexpr type_id id = type_id::t_short;
	};

	template <>
	struct type_block<int>
	{
		static constexpr type_id id = type_id::t_int;
	};

	template <>
	struct type_block<int64_t>
	{
		static constexpr type_id id = type_id::t_int64;
	};

	template <>
	struct type_block<float>
	{
		static constexpr type_id id = type_id::t_float;
	};

	template <>
	struct type_block<double>
	{
		static constexpr type_id id = type_id::t_double;
	};
}