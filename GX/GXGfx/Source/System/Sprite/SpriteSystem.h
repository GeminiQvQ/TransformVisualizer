#pragma once

// Project Includes
#include <GXGfx/Engine/EngineHandler.h>
#include <GXGfx/System/Sprite/SpriteTypes.h>
#include <GXGfx/API.h>
#include <GXLib/Graphics/PixelFormat.h>

// Stdlib Includes
#include <string_view>

// Forward Declarations
namespace GX::Gfx
{
	class Engine;
}

namespace GX::Gfx
{
	class GXGFX_API SpriteSystem : public EngineHandler
	{
	public:
		// Construction & Destruction
						SpriteSystem	(Engine& engine);
						~SpriteSystem	();

		// Registration
		SpriteHandle	register_sprite	(std::string_view filepath);
		SpriteHandle	register_sprite	(const void* data, size_t width, size_t height, PixelFormat format);

		// Instancing
		void			add_instance	(SpriteHandle handle, const SpriteInstanceParameters& parameters);

	protected:
		// EngineHandler Interface
		virtual void	on_init			(Context& context) override;
		virtual void	on_deinit		(Context& context) override;

		virtual void	on_draw			(DeviceContext& device_context) override;
		virtual void	on_draw_end		(DeviceContext& device_context) override;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}