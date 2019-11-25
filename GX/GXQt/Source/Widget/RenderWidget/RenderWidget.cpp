// Local Includes
#include <GXQt/Widget/RenderWidget/RenderWidget.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11Context.h>
#include <GXGfx/API/Device.h>
#include <GXGfx/API/DeviceContext.h>
#include <GXGfx/API/RenderTarget.h>
#include <GXGfx/API/Viewport.h>
#include <GXLib/StdExt/Invoke.h>

// Stdlib Includes
#include <vector>

// Third-Party Includes
#include <QResizeEvent>
#include <QTimer>

namespace GX::QT::RenderWidget
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------
	
	struct Widget::Internal
	{
		UPtr<GX::Gfx::Context>		gfx_context;
		UPtr<GX::Gfx::RenderTarget> gfx_render_target;

		std::vector<Handler*>		handlers;
		QTimer						draw_timer;
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	Widget::Widget(API api, QWidget* parent)
		: QWidget {parent}
		, m {std::make_unique<Internal>()}
	{
		// Widget.
		setAttribute(Qt::WA_PaintOnScreen, true);

		// API.
		switch (api)
		{
		case API::D3D11:
			m->gfx_context = std::make_unique<GX::Gfx::D3D11Context>((HWND)winId());
			break;

		default:
			assert(false);
			break;
		}

		// Timer.
		connect(&m->draw_timer, &QTimer::timeout, this, &Widget::draw);
	}

	Widget::~Widget()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Drawing
	//-----------------------------------------------------------------------------------------------------

	void Widget::draw()
	{
		invoke_conditional(m->handlers, &Handler::on_draw);
		m->gfx_context->device().present();
	}

	void Widget::start_draw_timer(int interval)
	{
		m->draw_timer.start(interval);
	}

	void Widget::stop_draw_timer()
	{
		m->draw_timer.stop();
	}

	//-----------------------------------------------------------------------------------------------------
	// Handlers
	//-----------------------------------------------------------------------------------------------------

	void Widget::add_handler(Handler& handler)
	{
		assert(std::find(m->handlers.begin(), m->handlers.end(), &handler) == m->handlers.end());
		m->handlers.emplace_back(&handler);
	}
	
	void Widget::remove_handler(Handler& handler)
	{
		const auto it = std::find(m->handlers.begin(), m->handlers.end(), &handler);
		assert(it != m->handlers.end());
		m->handlers.erase(it);
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	Gfx::Context& Widget::gfx_context()
	{
		return *m->gfx_context;
	}

	const Gfx::Context& Widget::gfx_context() const
	{
		return *m->gfx_context;
	}

	//-----------------------------------------------------------------------------------------------------
	// QPaintDevice Interface
	//-----------------------------------------------------------------------------------------------------

	QPaintEngine* Widget::paintEngine() const
	{
		return nullptr;
	}

	//-----------------------------------------------------------------------------------------------------
	// Qt Events
	//-----------------------------------------------------------------------------------------------------

	void Widget::resizeEvent(QResizeEvent* event)
	{
		int width = event->size().width();
		int height = event->size().height();

		if (m->gfx_render_target != nullptr)
		{
			m->gfx_context->device_context().set_render_target(nullptr);
			m->gfx_render_target = nullptr;
		}

		m->gfx_context->device_context().set_viewport({0.0f, 0.0f, (float)width, (float)height});
		m->gfx_context->device().resize_backbuffer(width, height);

		m->gfx_render_target = m->gfx_context->create_render_target(GX::Gfx::RenderTargetParametersBackbuffer{});
		m->gfx_context->device_context().set_render_target(m->gfx_render_target.get());
	}
}