#pragma once

// Project Includes
#include <GXQt/Widget/RenderWidget/RenderWidgetHandler.h>
#include <GXQt/Widget/RenderWidget/RenderWidgetTypes.h>
#include <GXQt/API.h>

// Third-Party Includes
#include <QWidget>

// Forward Declarations
namespace GX::Gfx
{
	class Context;
}

namespace GX::QT::RenderWidget
{
	class GXQT_API Widget : public QWidget
	{
	public:
		// Construction & Destruction
								Widget				(API api, QWidget* parent = nullptr);
								~Widget				();

		// Drawing
		void					draw				();
		void					start_draw_timer	(int interval);
		void					stop_draw_timer		();

		// Handlers
		void					add_handler			(Handler& handler);
		void					remove_handler		(Handler& handler);

		// Accessors
		Gfx::Context&			gfx_context			();
		const Gfx::Context&		gfx_context			() const;

		// QPaintDevice Interface
		virtual QPaintEngine*	paintEngine			() const override;

	protected:
		// Qt Events
		virtual void			resizeEvent			(QResizeEvent *event) override;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}