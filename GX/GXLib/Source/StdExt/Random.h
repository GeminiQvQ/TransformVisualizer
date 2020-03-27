#pragma once

namespace GX
{
	template <class Engine>
	typename Engine::result_type random(Engine& engine);

	template <class Engine>
	typename Engine::result_type random(Engine& engine, typename Engine::result_type min, typename Engine::result_type max);
}

// Inline Includes
#include <GXLib/StdExt/Random.inl>