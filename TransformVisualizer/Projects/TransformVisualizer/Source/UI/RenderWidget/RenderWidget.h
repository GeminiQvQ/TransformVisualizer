#pragma once

// Project Includes
#include <GXQt/Widget/RenderWidget/RenderWidget.h>
#include <TransformVisualizer/API.h>

// Forward Declarations
namespace GX::QT
{
	struct MouseMoveInputParameters;
	struct MouseWheelInputParameters;
}

namespace TransformVisualizer
{
	class KernelInterface;
}

namespace TransformVisualizer
{
	class RenderWidget : public GX::QT::RenderWidget::Widget
					   , public GX::QT::RenderWidget::Handler
	{
	public:
		// Construction & Destruction
							RenderWidget		(KernelInterface& kernel_interface, QWidget* parent = nullptr);
							~RenderWidget		();

	protected:
		// Handler Interface
		virtual bool		on_draw				() override;

	private:
		// Camera Controls
		void				on_mouse_lb_on		();
		void				on_mouse_lb_off		();
		void				on_mouse_rb_on		();
		void				on_mouse_rb_off		();
		void				on_mouse_lb_move	(const GX::QT::MouseMoveInputParameters& parameters);
		void				on_mouse_rb_move	(const GX::QT::MouseMoveInputParameters& parameters);
		void				on_mouse_wheel		(const GX::QT::MouseWheelInputParameters& parameters);
		void				on_mouse_wheel_end	();
		void				on_reset_camera		();

		// Helpers
		void				setup_gfx			();

		struct Internal;
		const UPtr<Internal> m;
	};
}