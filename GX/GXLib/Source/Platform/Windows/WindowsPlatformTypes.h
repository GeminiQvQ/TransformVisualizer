#pragma once

// Platform Includes
#include <GXLib/Platform/PlatformTypes.h>

namespace GX
{
	// Type Conversion
	int get_windows_key				(Key key);
	Key get_platform_key			(int key);

	int get_windows_modifier		(Modifier modifier);
	int get_windows_mouse_button	(MouseButton button);
}