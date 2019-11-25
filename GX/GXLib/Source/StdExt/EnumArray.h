#pragma once

// Stdlib Includes
#include <array>

namespace GX
{
	template <class EnumType, class ValueType, size_t Size>
	class EnumArray : public std::array<ValueType, Size>
	{
	public:
		using std::array<ValueType, Size>::operator[];

		typename std::array<ValueType, Size>::reference operator[] (EnumType type)
		{
			return (*this)[static_cast<size_type>(type)];
		}

		typename std::array<ValueType, Size>::const_reference operator[] (EnumType type) const
		{
			return (*this)[static_cast<size_type>(type)];
		}
	};
}