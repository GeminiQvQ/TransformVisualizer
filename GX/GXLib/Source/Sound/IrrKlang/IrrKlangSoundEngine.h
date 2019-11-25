#pragma once

// Project Includes
#include <GXLib/Sound/SoundEngine.h>

// Stdlib Includes
#include <memory>

namespace GX
{
	class IrrKlangSoundEngine : public SoundEngine
	{
	public:
		// Construction & Destruction
						IrrKlangSoundEngine		();
						~IrrKlangSoundEngine	();

		// Sound Engine Interface
		virtual void	play_sound				(std::string_view filepath, bool loop) override;

	private:
		struct Internal;
		const std::unique_ptr<Internal> m;
	};
}