#pragma once

// Forward Declarations
namespace GX::Gfx
{
	class Context;
	class DeviceContext;
}

namespace GX::Gfx
{
	class EngineHandler
	{
	protected:
		virtual void on_init		(Context& context);
		virtual void on_deinit		(Context& context);

		virtual void on_draw_begin	(DeviceContext& device_context);
		virtual void on_draw		(DeviceContext& device_context);
		virtual void on_draw_end	(DeviceContext& device_context);

		friend class Engine;
	};
}