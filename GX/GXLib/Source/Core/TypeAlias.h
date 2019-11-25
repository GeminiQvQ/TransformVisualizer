#pragma once

// Stdlib Includes
#include <memory>

namespace GX
{	
	template <typename T>
	using SPtr = std::shared_ptr<T>;

	template <typename T>
	using SCPtr = std::shared_ptr<const T>;

	template <typename T>
	using WPtr = std::weak_ptr<T>;

	template <typename T>
	using UPtr = std::unique_ptr<T>;
}