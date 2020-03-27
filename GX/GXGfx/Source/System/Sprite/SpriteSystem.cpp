// Local Includes
#include <GXGfx/System/Sprite/SpriteSystem.h>

// Project Includes
#include <GXGfx/API/BlendState.h>
#include <GXGfx/API/ConstantBuffer.h>
#include <GXGfx/API/Context.h>
#include <GXGfx/API/DeviceContext.h>
#include <GXGfx/API/SamplerState.h>
#include <GXGfx/API/Shader.h>
#include <GXGfx/API/Texture.h>
#include <GXGfx/API/VertexBuffer.h>
#include <GXGfx/API/Viewport.h>
#include <GXGfx/Engine/Engine.h>
#include <GXLib/Data/Image.h>
#include <GXLib/Math/Matrix.h>

// Stdlib Includes
#include <algorithm>
#include <map>
#include <vector>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct SpriteSystem::Internal
	{
		struct SpriteAsset
		{
			SpriteHandle  handle;
			std::string	  filepath;
			UPtr<Texture> texture;
		};

		struct SpriteInstance
		{
			SpriteHandle			 handle;
			size_t					 index;
			SpriteInstanceParameters parameters;
		};

		struct Vertex
		{
			Vector3F pos;
			Vector2F uv;
		};

		struct ConstantBufferVS
		{
			Matrix projection_transform;
		};

		__declspec(align(16))
		struct ConstantBufferPS
		{
			Vector4F blend_color;
			bool	 is_grayscale;
		};

		using AssetList = std::vector<SpriteAsset>;
		using InstanceMap = std::map<SpriteSpace, std::vector<SpriteInstance>>;

		Engine&					engine;

		UPtr<VertexShader>		vertex_shader;
		UPtr<PixelShader>		pixel_shader;
		UPtr<VertexBuffer>		vertex_buffer;
		UPtr<SamplerState>		sampler_state_ps;
		UPtr<ConstantBuffer>	constant_buffer_vs;
		UPtr<ConstantBuffer>	constant_buffer_ps;
		UPtr<BlendState>		blend_state;

		AssetList				assets;
		InstanceMap				instances;

		Internal(Engine& engine)
			: engine {engine}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	SpriteSystem::SpriteSystem(Engine& engine)
		: m {std::make_unique<Internal>(engine)}
	{
	}

	SpriteSystem::~SpriteSystem()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Registration
	//-----------------------------------------------------------------------------------------------------

	SpriteHandle SpriteSystem::register_sprite(std::string_view filepath)
	{
		const auto it = std::find_if(m->assets.begin(), m->assets.end(), [filepath] (auto&& asset)
		{
			return (asset.filepath == filepath);
		});

		if (it != m->assets.end())
		{
			return it->handle;
		}

		Image image;
		if (!image.load(filepath))
		{
			return SpriteHandleInvalid;
		}

		const auto handle = register_sprite(image.pixels(), image.width(), image.height(), image.format());

		m->assets.back().filepath = filepath;

		return handle;
	}

	SpriteHandle SpriteSystem::register_sprite(const void* data, size_t width, size_t height, PixelFormat format)
	{
		TextureParameters texture_parameters {};
		texture_parameters.width = width;
		texture_parameters.height = height;
		texture_parameters.format = format;
		texture_parameters.data = data;
		
		auto texture = m->engine.context().create_texture(texture_parameters);
		if (texture == nullptr)
		{
			return SpriteHandleInvalid;
		}

		const auto handle = SpriteHandle(m->assets.size());
		m->assets.emplace_back(Internal::SpriteAsset{handle, "", std::move(texture)});
		
		return handle;
	}

	//-----------------------------------------------------------------------------------------------------
	// Instancing
	//-----------------------------------------------------------------------------------------------------

	void SpriteSystem::add_instance(SpriteHandle handle, const SpriteInstanceParameters& parameters)
	{
		auto& instances = m->instances[parameters.space];
		instances.emplace_back(Internal::SpriteInstance{handle, instances.size(), parameters});
	}

	//-----------------------------------------------------------------------------------------------------
	// EngineHandler Interface
	//-----------------------------------------------------------------------------------------------------

	void SpriteSystem::on_init(Context& context)
	{
		// Shaders.
		m->vertex_shader = context.create_vertex_shader(L"Data\\Shaders\\SpriteVS.hlsl");
		m->pixel_shader = context.create_pixel_shader(L"Data\\Shaders\\SpritePS.hlsl");

		// Vertex buffer.
		std::vector<VertexBufferLayoutEntry> vertex_layout
		{
			{ "POSITION", PixelFormat::R32G32B32Float, 12 },
			{ "TEXCOORD", PixelFormat::R32G32Float, 8 }
		};

		VertexBufferParameters vertex_buffer_parameters {};
		vertex_buffer_parameters.layout = vertex_layout;
		vertex_buffer_parameters.shader = m->vertex_shader.get();
		vertex_buffer_parameters.size = sizeof(Internal::Vertex) * 6;
		vertex_buffer_parameters.stride = sizeof(Internal::Vertex);
		m->vertex_buffer = context.create_vertex_buffer(vertex_buffer_parameters);

		// Constant buffers.
		ConstantBufferParameters constant_buffer_vs_parameters {};
		constant_buffer_vs_parameters.size = sizeof(Internal::ConstantBufferVS);
		m->constant_buffer_vs = context.create_constant_buffer(constant_buffer_vs_parameters);

		ConstantBufferParameters constant_buffer_ps_parameters {};
		constant_buffer_ps_parameters.size = sizeof(Internal::ConstantBufferPS);
		m->constant_buffer_ps = context.create_constant_buffer(constant_buffer_ps_parameters);

		// Sampler state.
		m->sampler_state_ps = context.create_sampler_state(SamplerStateParameters{});

		// Blend state.
		BlendStateParameters blend_state_parameters {};
		blend_state_parameters.enabled = true;
		blend_state_parameters.blend_src = BlendType::SrcAlpha;
		blend_state_parameters.blend_dst = BlendType::InvSrcAlpha;
		blend_state_parameters.blend_op = BlendOp::Add;
		blend_state_parameters.blend_alpha_src = BlendType::SrcAlpha;
		blend_state_parameters.blend_alpha_dst = BlendType::InvSrcAlpha;
		blend_state_parameters.blend_alpha_op = BlendOp::Add;
		m->blend_state = context.create_blend_state(blend_state_parameters);
	}

	void SpriteSystem::on_deinit(Context& /*context*/)
	{
		m->vertex_shader.reset();
		m->pixel_shader.reset();
		m->vertex_buffer.reset();
		m->sampler_state_ps.reset();
		m->constant_buffer_vs.reset();
		m->constant_buffer_ps.reset();
		m->blend_state.reset();

		m->assets.clear();
	}

	//-----------------------------------------------------------------------------------------------------

	void SpriteSystem::on_draw(DeviceContext& device_context)
	{
		device_context.set_vertex_shader(*m->vertex_shader);
		device_context.set_pixel_shader(*m->pixel_shader);
		device_context.set_sampler_state_ps(*m->sampler_state_ps);
		device_context.set_constant_buffer_vs(*m->constant_buffer_vs);
		device_context.set_constant_buffer_ps(*m->constant_buffer_ps);
		device_context.set_blend_state(m->blend_state.get());

		const auto draw = [this, &device_context] (std::vector<Internal::SpriteInstance>& instances, float y_factor)
		{
			std::sort(instances.begin(), instances.end(), [] (auto&& a, auto&& b)
			{
				if (a.parameters.z_order < b.parameters.z_order) return true;
				if (a.parameters.z_order > b.parameters.z_order) return false;
				return (a.index < b.index);
			});

			for (auto&& instance : instances)
			{
				auto asset_it = std::find_if(m->assets.begin(), m->assets.end(), [&instance] (auto&& asset)
				{
					return (asset.handle == instance.handle);
				});

				const auto center = instance.parameters.position;
				const auto size = instance.parameters.size;
				const auto anchor_offset = get_anchor_offset_from_center(instance.parameters.anchor);

				const auto tl = Vector3F{center.x - size.x * 0.5f + size.x * anchor_offset.x, center.y * y_factor + size.y * 0.5f + size.y * anchor_offset.y, 0.0f};
				const auto tr = Vector3F{center.x + size.x * 0.5f + size.x * anchor_offset.x, center.y * y_factor + size.y * 0.5f + size.y * anchor_offset.y, 0.0f};
				const auto bl = Vector3F{center.x - size.x * 0.5f + size.x * anchor_offset.x, center.y * y_factor - size.y * 0.5f + size.y * anchor_offset.y, 0.0f};
				const auto br = Vector3F{center.x + size.x * 0.5f + size.x * anchor_offset.x, center.y * y_factor - size.y * 0.5f + size.y * anchor_offset.y, 0.0f};

				/*
				const float uvx0 = 0.75f / size.x;
				const float uvx1 = 1.0f - uvx0;
				const float uvy0 = 0.75f / size.y;
				const float uvy1 = 1.0f - uvy0;
				*/

				const float uvx0 = 0.0f;
				const float uvx1 = 1.0f;
				const float uvy0 = 0.0f;
				const float uvy1 = 1.0f;

				const auto vertices = std::array<Internal::Vertex, 6>
				{
					Internal::Vertex{tl, Vector2F{uvx0, uvy0}},
					Internal::Vertex{tr, Vector2F{uvx1, uvy0}},
					Internal::Vertex{bl, Vector2F{uvx0, uvy1}},
					Internal::Vertex{bl, Vector2F{uvx0, uvy1}},
					Internal::Vertex{tr, Vector2F{uvx1, uvy0}},
					Internal::Vertex{br, Vector2F{uvx1, uvy1}}
				};

				Internal::ConstantBufferPS constant_buffer_data_ps;
				constant_buffer_data_ps.blend_color = instance.parameters.blend_color;
				constant_buffer_data_ps.is_grayscale = instance.parameters.is_grayscale;
				m->constant_buffer_ps->set(&constant_buffer_data_ps);

				asset_it->texture->bind(device_context);
				m->vertex_buffer->update(device_context, &vertices[0], vertices.size() * sizeof(Internal::Vertex));
				m->vertex_buffer->draw(device_context);
			}
		};

		Internal::ConstantBufferVS constant_buffer_data_vs;
		const auto resolution = m->engine.resolution();

		constant_buffer_data_vs.projection_transform = Matrix::create_orthographic(Vector2F{resolution.x, resolution.y}, 0.0f, 1.0f);
		m->constant_buffer_vs->set(&constant_buffer_data_vs);
		draw(m->instances[SpriteSpace::World], 1.0f);

		constant_buffer_data_vs.projection_transform = Matrix::create_orthographic(Vector2F{resolution.x, resolution.y}, 0.0f, 1.0f) * Matrix::create_translation(Vector3F{-1.0f, 1.0f, 0.0f});
		m->constant_buffer_vs->set(&constant_buffer_data_vs);
		draw(m->instances[SpriteSpace::Screen], -1.0f);
	}

	void SpriteSystem::on_draw_end(DeviceContext& /*device_context*/)
	{
		m->instances.clear();
	}
}