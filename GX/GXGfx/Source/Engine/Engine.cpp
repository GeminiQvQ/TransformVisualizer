// Local Includes
#include <GXGfx/Engine/Engine.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11Context.h>
#include <GXGfx/API/Device.h>
#include <GXGfx/Engine/EngineHandler.h>
#include <GXGfx/System/Sprite/SpriteSystem.h>
#include <GXGfx/System/Text/TextSystem.h>

// Stdlib includes
#include <cassert>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct Engine::Internal
	{
		UPtr<Context>				context;
		std::vector<EngineHandler*>	handlers;
		
		Vector2F					resolution;

		UPtr<SpriteSystem>			sprite_system;
		UPtr<TextSystem>			text_system;

		Internal()
			: context {}
			, handlers {}
			, resolution {500.0f, 500.0}
			, sprite_system {}
			, text_system {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	Engine::Engine(API api, size_t window_handle)
		: m {std::make_unique<Internal>()}
	{
		// API.
		switch (api)
		{
		case API::D3D11:
			m->context = std::make_unique<D3D11Context>((HWND(window_handle)));
			break;

		default:
			assert(false);
			break;
		}

		// Systems.
		m->sprite_system = std::make_unique<SpriteSystem>(*this);
		m->handlers.push_back(m->sprite_system.get());
		m->text_system = std::make_unique<TextSystem>(*this);
		m->handlers.push_back(m->text_system.get());

		for (auto&& handler : m->handlers)
		{
			handler->on_init(*m->context);
		}
	}

	Engine::~Engine()
	{
		for (auto&& handler : m->handlers)
		{
			handler->on_deinit(*m->context);
		}
	}

	//-----------------------------------------------------------------------------------------------------
	// Drawing
	//-----------------------------------------------------------------------------------------------------

	void Engine::draw()
	{
		for (auto handler : m->handlers)
		{
			handler->on_draw_begin(m->context->device_context());
		}

		for (auto handler : m->handlers)
		{
			handler->on_draw(m->context->device_context());
		}

		m->context->device().present();

		for (auto handler : m->handlers)
		{
			handler->on_draw_end(m->context->device_context());
		}
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	void Engine::set_resolution(Vector2F resolution)
	{
		m->resolution = resolution;
	}

	const Vector2F& Engine::resolution() const
	{
		return m->resolution;
	}

	//-----------------------------------------------------------------------------------------------------
	// Systems
	//-----------------------------------------------------------------------------------------------------

	SpriteSystem& Engine::sprite()
	{
		return *m->sprite_system;
	}

	const SpriteSystem& Engine::sprite() const
	{
		return *m->sprite_system;
	}

	TextSystem& Engine::text()
	{
		return *m->text_system;
	}

	const TextSystem& Engine::text() const
	{
		return *m->text_system;
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	Context& Engine::context()
	{
		return *m->context;
	}

	const Context& Engine::context() const
	{
		return *m->context;
	}
};