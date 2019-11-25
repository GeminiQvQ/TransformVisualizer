// Local Includes
#include <GXLib/Sound/IrrKlang/IrrKlangSoundEngine.h>

// Third-Party Includes
#include <irrKlang.h>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct IrrKlangSoundEngine::Internal
	{
		irrklang::ISoundEngine* engine;

		Internal()
			: engine {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	IrrKlangSoundEngine::IrrKlangSoundEngine()
		: m {std::make_unique<Internal>()}
	{
		m->engine = irrklang::createIrrKlangDevice();
	}

	IrrKlangSoundEngine::~IrrKlangSoundEngine()
	{
		m->engine->drop();
	}

	//-----------------------------------------------------------------------------------------------------
	// Sound Engine Interface
	//-----------------------------------------------------------------------------------------------------

	void IrrKlangSoundEngine::play_sound(std::string_view filepath, bool loop)
	{
		m->engine->play2D(filepath.data(), loop);
	}
}