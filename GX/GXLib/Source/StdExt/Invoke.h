#pragma once

// Stdlib Includes
#include <functional>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Multi-Invoke
	//-----------------------------------------------------------------------------------------------------

	template <class Container, class Callback, class... Args>
	void invoke(const Container& container, const Callback& callback, Args... args);

	template <class It, class Callback, class... Args>
	void invoke(It first, It end, const Callback& callback, Args... args);

	template <class Container, class Callback, class... Args>
	bool invoke_conditional(const Container& container, const Callback& callback, Args... args);

	template <class It, class Callback, class... Args>
	bool invoke_conditional(It first, It end, const Callback& callback, Args... args);
}

// Inline Includes
#include <GXLib/StdExt/Invoke.inl>