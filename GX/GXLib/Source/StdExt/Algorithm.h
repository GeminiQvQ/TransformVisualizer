#pragma once

// Stdlib Includes
#include <algorithm>

namespace GX
{
	template <class C, class T>
	bool contains(C&& container, T&& value)
	{
		return (std::find(std::begin(container), std::end(container), value) != std::end(container));
	}

	template <class It, class T>
	bool contains(It&& from, It&& to, T&& value)
	{
		return (std::find(from, to, value) != std::end(container));
	}

	template <class C, class Pred>
	auto remove_and_erase_if(C&& container, Pred&& pred)
	{
		return container.erase(std::remove_if(std::begin(container), std::end(container), pred), std::end(container));
	}
}