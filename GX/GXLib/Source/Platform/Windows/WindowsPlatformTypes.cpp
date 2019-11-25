// Local Includes
#include <GXLib/Platform/Windows/WindowsPlatformTypes.h>

// Stdlib Includes
#include <algorithm>
#include <map>

// Third-Party Includes
#include <windows.h>

namespace GX
{
	const std::map<Key, int>& get_windows_key_map()
	{
		static const std::map<Key, int> s_map
		{
			{ Key::A, 0x41 },
			{ Key::B, 0x42 },
			{ Key::C, 0x43 },
			{ Key::D, 0x44 },
			{ Key::E, 0x45 },
			{ Key::F, 0x46 },
			{ Key::G, 0x47 },
			{ Key::H, 0x48 },
			{ Key::I, 0x49 },
			{ Key::J, 0x4A },
			{ Key::K, 0x4B },
			{ Key::L, 0x4C },
			{ Key::M, 0x4D },
			{ Key::N, 0x4E },
			{ Key::O, 0x4F },
			{ Key::P, 0x50 },
			{ Key::Q, 0x51 },
			{ Key::R, 0x52 },
			{ Key::S, 0x53 },
			{ Key::T, 0x54 },
			{ Key::U, 0x55 },
			{ Key::V, 0x56 },
			{ Key::W, 0x57 },
			{ Key::X, 0x58 },
			{ Key::Y, 0x59 },
			{ Key::Z, 0x5A },
			{ Key::N0, 0x30 },
			{ Key::N1, 0x31 },
			{ Key::N2, 0x32 },
			{ Key::N3, 0x33 },
			{ Key::N4, 0x34 },
			{ Key::N5, 0x35 },
			{ Key::N6, 0x36 },
			{ Key::N7, 0x37 },
			{ Key::N8, 0x38 },
			{ Key::N9, 0x39 },
			{ Key::K0, VK_NUMPAD0 },
			{ Key::K1, VK_NUMPAD1 },
			{ Key::K2, VK_NUMPAD2 },
			{ Key::K3, VK_NUMPAD3 },
			{ Key::K4, VK_NUMPAD4 },
			{ Key::K5, VK_NUMPAD5 },
			{ Key::K6, VK_NUMPAD6 },
			{ Key::K7, VK_NUMPAD7 },
			{ Key::K8, VK_NUMPAD8 },
			{ Key::K9, VK_NUMPAD9 },
			{ Key::F1, VK_F1 },
			{ Key::F2, VK_F2 },
			{ Key::F3, VK_F3 },
			{ Key::F4, VK_F4 },
			{ Key::F5, VK_F5 },
			{ Key::F6, VK_F6 },
			{ Key::F7, VK_F7 },
			{ Key::F8, VK_F8 },
			{ Key::F9, VK_F9 },
			{ Key::F10, VK_F10 },
			{ Key::F11, VK_F11 },
			{ Key::F12, VK_F12 },
			{ Key::Left, VK_LEFT },
			{ Key::Right, VK_RIGHT },
			{ Key::Up, VK_UP },
			{ Key::Down, VK_DOWN },
			{ Key::Esc, VK_ESCAPE },
			{ Key::Space, VK_SPACE },
			{ Key::Backspace, VK_BACK },
			{ Key::Return, VK_RETURN }
		};
		return s_map;
	}

	int get_windows_key(Key key)
	{
		auto it = get_windows_key_map().find(key);
		return (it != get_windows_key_map().end() ? it->second : -1);
	}

	Key get_platform_key(int key)
	{
		auto it = std::find_if(get_windows_key_map().begin(), get_windows_key_map().end(), [key] (const auto& kv)
		{
			return (kv.second == key);
		});
		return (it != get_windows_key_map().end() ? it->first : Key::Invalid);
	}
}