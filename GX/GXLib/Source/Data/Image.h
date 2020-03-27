#pragma once

// Project Includes
#include <GXLib/Graphics/PixelFormat.h>
#include <GXLib/API.h>

// Stdlib Includes
#include <cstdint>
#include <string_view>

namespace GX
{
	class GXLIBS_API Image
	{
	public:
		// Construction & Destruction
						Image	();
						~Image	();

		// Load & Save
		bool			load	(std::string_view filepath);
		bool			save	(std::string_view filepath);

		// Properties
		size_t			width	() const;
		size_t			height	() const;
		PixelFormat		format	() const;

		// Accessors
		uint8_t*		pixels	();
		const uint8_t*	pixels	() const;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}