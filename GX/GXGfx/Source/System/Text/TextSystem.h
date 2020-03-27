#pragma once

// Project Includes
#include <GXGfx/Engine/EngineHandler.h>
#include <GXGfx/System/Text/TextTypes.h>
#include <GXGfx/API.h>

// Stdlib Includes
#include <string_view>

// Forward Declarations
namespace GX::Gfx
{
	class Engine;
}

namespace GX::Gfx
{
	class GXGFX_API TextSystem : public EngineHandler
	{
	public:
		// Construction & Destruction
						TextSystem		(Engine& engine);
						~TextSystem		();

		// Registration
		FontHandle		register_font	(std::string_view filepath, size_t size);

		// Instancing
		void			add_instance	(FontHandle handle, std::string_view text, const TextInstanceParameters& parameters);

	protected:
		// EngineHandler Interface
		virtual void	on_deinit		(Context& context) override;

	private:
		// Helpers
		bool			cache_glyph		(FontHandle handle, char glyph);

		struct Internal;
		const UPtr<Internal> m;
	};
}