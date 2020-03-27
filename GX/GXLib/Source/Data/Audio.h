#pragma once

// Project Includes
#include <GXLib/API.h>

// Stdlib Includes
#include <cstdint>
#include <string_view>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	using AudioHandle = intptr_t;
	constexpr AudioHandle AudioHandleInvalid = AudioHandle(0);

	//-----------------------------------------------------------------------------------------------------
	// AudioEngine
	//-----------------------------------------------------------------------------------------------------

	class GXLIBS_API AudioEngine
	{
	public:
		// Construction & Destruction
					AudioEngine		();
					~AudioEngine	();

		// Resources
		AudioHandle	register_audio	(std::string_view filepath);
		void		release_audio	(AudioHandle& handle);

		// Engine
		void		set_volume		(float volume);
		float		volume			() const;

		// Audio
		void		play_sound		(std::string_view filepath, float volume);
		void		play_sound		(AudioHandle handle, bool loop);
		void		stop_sound		(AudioHandle handle);

		void		set_volume		(AudioHandle handle, float volume);
		float		volume			(AudioHandle handle) const;

		void		set_position	(AudioHandle handle, int position);
		int			position		(AudioHandle handle) const;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}