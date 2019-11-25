#pragma once

// Stdlib Includes
#include <type_traits>
#include <variant>

namespace GX
{
	template <typename>
	struct VariantIndexTag
	{
	};

	template <typename, typename>
	struct VariantIndex;

	template <typename T, typename... Ts>
	struct VariantIndex<T, std::variant<Ts...>>
		: std::integral_constant<size_t, std::variant<VariantIndexTag<Ts>...>(VariantIndexTag<T>()).index()>
	{
	};

	template <typename T, typename... Ts>
	bool is_variant(const std::variant<Ts...>& variant)
	{
		return (variant.index() == VariantIndex<T, std::variant<Ts...>>::value);
	}
}