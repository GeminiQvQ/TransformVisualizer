// Local Includes
#include <GXLib/Data/Audio.h>

// Third-Party Includes
#include <irrKlang.h>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------
	
	struct AudioEngine::Internal
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

	AudioEngine::AudioEngine()
		: m {std::make_unique<Internal>()}
	{
		m->engine = irrklang::createIrrKlangDevice();
	}

	AudioEngine::~AudioEngine()
	{
		m->engine->drop();
	}

	//-----------------------------------------------------------------------------------------------------
	// Resources
	//-----------------------------------------------------------------------------------------------------

	AudioHandle AudioEngine::register_audio(std::string_view filepath)
	{
		auto sound = m->engine->play2D(filepath.data(), false, true, true);
		return (AudioHandle)sound;
	}

	void AudioEngine::release_audio(AudioHandle& handle)
	{
		auto*& sound = (irrklang::ISound*&)handle;
		sound->stop();
		sound->drop();
		sound = nullptr;
	}

	//-----------------------------------------------------------------------------------------------------
	// Engine
	//-----------------------------------------------------------------------------------------------------

	void AudioEngine::set_volume(float volume)
	{
		m->engine->setSoundVolume(volume);
	}

	float AudioEngine::volume() const
	{
		return m->engine->getSoundVolume();
	}

	//-----------------------------------------------------------------------------------------------------
	// Audio
	//-----------------------------------------------------------------------------------------------------

	void AudioEngine::play_sound(std::string_view filepath, float volume)
	{
		auto sound = m->engine->play2D(filepath.data());
		sound->setVolume(volume);
	}

	void AudioEngine::play_sound(AudioHandle handle, bool loop)
	{
		auto sound = (irrklang::ISound*)handle;

		sound->setIsLooped(loop);
		sound->setIsPaused(false);
		sound->setPlayPosition(0);
	}

	void AudioEngine::stop_sound(AudioHandle handle)
	{
		auto sound = (irrklang::ISound*)handle;
		
		sound->setIsPaused(true);
	}

	//-----------------------------------------------------------------------------------------------------

	void AudioEngine::set_volume(AudioHandle handle, float volume)
	{
		auto sound = (irrklang::ISound*)handle;

		sound->setVolume(volume);
	}

	float AudioEngine::volume(AudioHandle handle) const
	{
		auto sound = (irrklang::ISound*)handle;

		return sound->getVolume();
	}

	//-----------------------------------------------------------------------------------------------------

	void AudioEngine::set_position(AudioHandle handle, int position)
	{
		auto sound = (irrklang::ISound*)handle;
		sound->setPlayPosition(position);
	}

	int AudioEngine::position(AudioHandle handle) const
	{
		auto sound = (irrklang::ISound*)handle;
		return sound->getPlayPosition();
	}
}