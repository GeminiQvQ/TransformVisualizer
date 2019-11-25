#pragma once

// Stdlib Includes
#include <string_view>

namespace GX
{
	class SoundEngine
	{
	public:
		// Construction & Destruction
		virtual			~SoundEngine	() = default;

		// Sound Engine Interface
		virtual void	play_sound		(std::string_view filepath, bool loop) = 0;
	};
}