// Local Includes
#include <TransformVisualizer/UI/RenderWidget/RenderWidget.h>

// Project Includes
#include <GXGfx/API/BlendState.h>
#include <GXGfx/API/ConstantBuffer.h>
#include <GXGfx/API/Context.h>
#include <GXGfx/API/DeviceContext.h>
#include <GXGfx/API/Shader.h>
#include <GXGfx/API/VertexBuffer.h>
#include <GXLib/Graphics/Camera.h>
#include <GXLib/Math/Matrix.h>
#include <GXLib/Math/Vector.h>
#include <GXQt/System/Input/InputSystem.h>
#include <TransformVisualizer/Core/Transform/TransformContainer.h>
#include <TransformVisualizer/Core/Settings.h>
#include <TransformVisualizer/Core/State.h>
#include <TransformVisualizer/KernelInterface.h>

// Third-Party Includes
#include <QTimer>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Mesh Structs
	//-----------------------------------------------------------------------------------------------------

	struct Vertex
	{
		GX::Vector3F position;
		int			 color_index;
	};

	struct ConstantBufferVS
	{
		GX::Matrix world_transform;
		GX::Matrix view_transform;
		GX::Matrix projection_transform;
	};

	struct ConstantBufferPS
	{
		std::array<GX::Vector4F, 6> color;
	};

	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct RenderWidget::Internal
	{
		KernelInterface&				kernel_interface;

		UPtr<GX::Gfx::VertexBuffer>		vertex_buffer;
		UPtr<GX::Gfx::VertexShader>		vertex_shader;
		UPtr<GX::Gfx::PixelShader>		pixel_shader;
		UPtr<GX::Gfx::ConstantBuffer>	constant_buffer_vs;
		UPtr<GX::Gfx::ConstantBuffer>	constant_buffer_ps;
		UPtr<GX::Gfx::BlendState>		blend_state;

		QTimer							wheel_timer;

		Internal(KernelInterface& kernel_interface)
			: kernel_interface {kernel_interface}
		{
		}
	};
	
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	RenderWidget::RenderWidget(KernelInterface& kernel_interface, QWidget* parent)
		: GX::QT::RenderWidget::Widget {GX::QT::RenderWidget::API::D3D11, parent}
		, m {std::make_unique<Internal>(kernel_interface)}
	{
		// Widget.
		setFocusPolicy(Qt::StrongFocus);

		// Render widget.
		setup_gfx();
		add_handler(*this);
		start_draw_timer(16);

		// Wheel timer.
		m->wheel_timer.setSingleShot(true);
		m->wheel_timer.setInterval(100);
		connect(&m->wheel_timer, &QTimer::timeout, this, &RenderWidget::on_mouse_wheel_end);

		// Input bindings.
		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputMouseButton{Qt::LeftButton}, GX::QT::InputMouseMove{}}, this,
											 GX::QT::MouseMoveInputCallback{std::bind(&RenderWidget::on_mouse_lb_move, this, std::placeholders::_1)});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputMouseButton{Qt::RightButton}, GX::QT::InputMouseMove{}}, this,
											 GX::QT::MouseMoveInputCallback{std::bind(&RenderWidget::on_mouse_rb_move, this, std::placeholders::_1)});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputMouseWheel{}}, this,
											 GX::QT::MouseWheelInputCallback{std::bind(&RenderWidget::on_mouse_wheel, this, std::placeholders::_1)});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputMouseButton{Qt::LeftButton}, GX::QT::InputTriggerType::OnEnabled}, this,
											 GX::QT::TriggerInputCallback{std::bind(&RenderWidget::on_mouse_lb_on, this)});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputMouseButton{Qt::LeftButton}, GX::QT::InputTriggerType::OnDisabled}, this,
											 GX::QT::TriggerInputCallback{std::bind(&RenderWidget::on_mouse_lb_off, this)});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputMouseButton{Qt::RightButton}, GX::QT::InputTriggerType::OnEnabled}, this,
											 GX::QT::TriggerInputCallback{std::bind(&RenderWidget::on_mouse_rb_on, this)});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputMouseButton{Qt::RightButton}, GX::QT::InputTriggerType::OnDisabled}, this,
											 GX::QT::TriggerInputCallback{std::bind(&RenderWidget::on_mouse_rb_off, this)});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputKeyboardKey{Qt::Key_Control}, GX::QT::InputKeyboardKey{Qt::Key_R}}, this,
											 GX::QT::TriggerInputCallback{std::bind(&RenderWidget::on_reset_camera, this)});
	}
	
	RenderWidget::~RenderWidget()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Handler Interface
	//-----------------------------------------------------------------------------------------------------

	bool RenderWidget::on_draw()
	{
		const auto& settings = m->kernel_interface.settings();
		const auto& camera = m->kernel_interface.state().camera();

		const float fov = m->kernel_interface.settings().projection_fov;
		const float near_plane = m->kernel_interface.settings().projection_near_plane;
		const float far_plane = m->kernel_interface.settings().projection_far_plane;

		gfx_context().device_context().clear(settings.background_color.x, settings.background_color.y, settings.background_color.z, settings.background_color.w);

		ConstantBufferVS constant_buffer_data_vs;
		constant_buffer_data_vs.world_transform = m->kernel_interface.transform_container().world_transform()->matrix();
		constant_buffer_data_vs.view_transform = GX::Matrix::create_view(camera.position(), camera.target(), camera.up());
		constant_buffer_data_vs.projection_transform = GX::Matrix::create_perspective(GX::deg2rad(fov), (float)width() / (float)height(), near_plane, far_plane);
		m->constant_buffer_vs->set(&constant_buffer_data_vs);

		ConstantBufferPS constant_buffer_data_ps;
		constant_buffer_data_ps.color = settings.cube_color;
		m->constant_buffer_ps->set(&constant_buffer_data_ps);

		m->vertex_buffer->draw(gfx_context().device_context());

		return false;
	}

	//-----------------------------------------------------------------------------------------------------
	// Camera Controls
	//-----------------------------------------------------------------------------------------------------

	void RenderWidget::on_mouse_lb_on()
	{
		m->kernel_interface.state().begin_camera_change();
	}

	void RenderWidget::on_mouse_lb_off()
	{
		m->kernel_interface.state().end_camera_change();
	}

	void RenderWidget::on_mouse_rb_on()
	{
		m->kernel_interface.state().begin_camera_change();
	}

	void RenderWidget::on_mouse_rb_off()
	{
		m->kernel_interface.state().end_camera_change();
	}

	void RenderWidget::on_mouse_lb_move(const GX::QT::MouseMoveInputParameters& parameters)
	{
		auto& camera = m->kernel_interface.state().camera();
		
		camera.rotate_cw_around_target((parameters.pos - parameters.pos_prev).x() * 0.12f);
		camera.rotate_up_around_target((parameters.pos - parameters.pos_prev).y() * -0.12f);
	}

	void RenderWidget::on_mouse_rb_move(const GX::QT::MouseMoveInputParameters& parameters)
	{
		auto& camera = m->kernel_interface.state().camera();
		
		const auto delta_x = (parameters.pos.x() - parameters.pos_prev.x()) * (camera.position() - camera.target()).length() * 0.0008f;
		camera.move_left(delta_x);
		const auto delta_y = (parameters.pos.y() - parameters.pos_prev.y()) * (camera.position() - camera.target()).length() * 0.0008f;
		camera.move_up(delta_y);
	}

	void RenderWidget::on_mouse_wheel(const GX::QT::MouseWheelInputParameters& parameters)
	{
		if (!m->wheel_timer.isActive())
		{
			m->kernel_interface.state().begin_camera_change();
		}

		m->wheel_timer.start();

		auto& camera = m->kernel_interface.state().camera();
		
		camera.move_forward(parameters.delta * -0.001f);
	}

	void RenderWidget::on_mouse_wheel_end()
	{
		m->kernel_interface.state().end_camera_change();
	}

	void RenderWidget::on_reset_camera()
	{
		m->kernel_interface.state().begin_camera_change();
		m->kernel_interface.state().camera() = GX::Camera{GX::Vector3F{0.0f, 0.0f, -5.0f}, GX::Vector3F{}, GX::Vector3F{0.0f, 1.0f, 0.0f}};
		m->kernel_interface.state().end_camera_change();
	}

	//-----------------------------------------------------------------------------------------------------
	// Helpers
	//-----------------------------------------------------------------------------------------------------

	void RenderWidget::setup_gfx()
	{
		// Shaders.
		m->vertex_shader = gfx_context().create_vertex_shader(L"Data\\ColorVS.hlsl");
		m->pixel_shader = gfx_context().create_pixel_shader(L"Data\\ColorPS.hlsl");
	
		gfx_context().device_context().set_vertex_shader(*m->vertex_shader);
		gfx_context().device_context().set_pixel_shader(*m->pixel_shader);

		// Vertex buffer.
		GX::Vector3F ftl { -0.5f,  0.5f, -0.5f };
		GX::Vector3F ftr {  0.5f,  0.5f, -0.5f };
		GX::Vector3F fbl { -0.5f, -0.5f, -0.5f };
		GX::Vector3F fbr {  0.5f, -0.5f, -0.5f };
		GX::Vector3F btl { -0.5f,  0.5f,  0.5f };
		GX::Vector3F btr {  0.5f,  0.5f,  0.5f };
		GX::Vector3F bbl { -0.5f, -0.5f,  0.5f };
		GX::Vector3F bbr {  0.5f, -0.5f,  0.5f };

		std::vector<Vertex> vertices
		{
			{ftl, 0}, {ftr, 0}, {fbl, 0}, {fbl, 0}, {ftr, 0}, {fbr, 0}, // Front
			{btr, 1}, {btl, 1}, {bbr, 1}, {bbr, 1}, {btl, 1}, {bbl, 1}, // Back
			{btl, 2}, {ftl, 2}, {bbl, 2}, {bbl, 2}, {ftl, 2}, {fbl, 2}, // Left
			{ftr, 3}, {btr, 3}, {fbr, 3}, {fbr, 3}, {btr, 3}, {bbr, 3}, // Right
			{btl, 4}, {btr, 4}, {ftl, 4}, {ftl, 4}, {btr, 4}, {ftr, 4}, // Top
			{fbl, 5}, {fbr, 5}, {bbl, 5}, {bbl, 5}, {fbr, 5}, {bbr, 5}  // Bottom
		};

		std::vector<GX::Gfx::VertexBufferLayoutEntry> vertex_layout
		{
			{ "POSITION", GX::Gfx::VertexBufferFormat::R32G32B32Float, 12 },
			{ "COLOR_INDEX", GX::Gfx::VertexBufferFormat::R32SInt, 4 }
		};

		GX::Gfx::VertexBufferParameters vertex_buffer_parameters_triangle {};
		vertex_buffer_parameters_triangle.layout = vertex_layout;
		vertex_buffer_parameters_triangle.data = &vertices[0];
		vertex_buffer_parameters_triangle.shader = m->vertex_shader.get();
		vertex_buffer_parameters_triangle.size = (int)vertices.size() * sizeof(Vertex);
		vertex_buffer_parameters_triangle.stride = sizeof(Vertex);
		m->vertex_buffer = gfx_context().create_vertex_buffer(vertex_buffer_parameters_triangle);

		// Constant buffers.
		GX::Gfx::ConstantBufferParameters constant_buffer_vs_parameters {};
		constant_buffer_vs_parameters.size = sizeof(ConstantBufferVS);
		m->constant_buffer_vs = gfx_context().create_constant_buffer(constant_buffer_vs_parameters);
		gfx_context().device_context().set_constant_buffer_vs(*m->constant_buffer_vs);

		GX::Gfx::ConstantBufferParameters constant_buffer_ps_parameters {};
		constant_buffer_ps_parameters.size = sizeof(ConstantBufferPS);
		m->constant_buffer_ps = gfx_context().create_constant_buffer(constant_buffer_ps_parameters);
		gfx_context().device_context().set_constant_buffer_ps(*m->constant_buffer_ps);

		// Blend state.
		GX::Gfx::BlendStateParameters blend_state_parameters {};
		blend_state_parameters.enabled = true;
		blend_state_parameters.blend_src = GX::Gfx::BlendType::SrcAlpha;
		blend_state_parameters.blend_dst = GX::Gfx::BlendType::InvSrcAlpha;
		blend_state_parameters.blend_op = GX::Gfx::BlendOp::Add;
		blend_state_parameters.blend_alpha_src = GX::Gfx::BlendType::SrcAlpha;
		blend_state_parameters.blend_alpha_dst = GX::Gfx::BlendType::InvSrcAlpha;
		blend_state_parameters.blend_alpha_op = GX::Gfx::BlendOp::Add;
		m->blend_state = gfx_context().create_blend_state(blend_state_parameters);
		gfx_context().device_context().set_blend_state(m->blend_state.get());
	}
}