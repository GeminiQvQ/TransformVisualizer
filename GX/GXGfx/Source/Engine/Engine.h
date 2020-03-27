#pragma once

// Project Includes
#include <GXGfx/Engine/EngineTypes.h>
#include <GXGfx/API.h>
#include <GXLib/Math/Vector.h>

// Forward Declarations
namespace GX::Gfx
{
	class Context;
	class EngineHandler;
	class SpriteSystem;
	class TextSystem;
}

namespace GX::Gfx
{
	class GXGFX_API Engine
	{
	public:
		// Construction & Destruction
							Engine			(API api, size_t window_handle);
							~Engine			();

		// Drawing
		void				draw			();

		// Properties
		void				set_resolution	(Vector2F resolution);
		const Vector2F&		resolution		() const;

		// Systems
		SpriteSystem&		sprite			();
		const SpriteSystem& sprite			() const;
		TextSystem&			text			();
		const TextSystem&	text			() const;

		// Accessors
		Context&			context			();
		const Context&		context			() const;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}