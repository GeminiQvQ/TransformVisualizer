// Local Includes
#include <GXGfx/System/Text/TextSystem.h>

// Project Includes
#include <GXGfx/Engine/Engine.h>
#include <GXGfx/System/Sprite/SpriteSystem.h>

// Stdlib Includes
#include <algorithm>
#include <map>
#include <vector>

// Third-Party Includes
#include <ft2build.h>
#include <freetype/freetype.h>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct TextSystem::Internal
	{
		struct SpriteEntry
		{
			SpriteHandle handle;
			Vector2F	 size;
			Vector2F	 offset;
			float		 advance;
		};

		struct FontEntry
		{
			FontHandle					handle;
			std::string					filepath;
			size_t						size;
			FT_Face						ft_face;
			float						height;
			std::map<char, SpriteEntry> sprite_map;
		};

		Engine&					engine;

		FT_Library				ft_library;

		std::vector<FontEntry>	fonts;

		Internal(Engine& engine)
			: engine {engine}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	TextSystem::TextSystem(Engine& engine)
		: m {std::make_unique<Internal>(engine)}
	{
		FT_Init_FreeType(&m->ft_library);
	}

	TextSystem::~TextSystem()
	{
		for (auto&& font : m->fonts)
		{
			FT_Done_Face(font.ft_face);
		}

		FT_Done_FreeType(m->ft_library);
	}

	//-----------------------------------------------------------------------------------------------------
	// Registration
	//-----------------------------------------------------------------------------------------------------

	FontHandle TextSystem::register_font(std::string_view filepath, size_t size)
	{
		const auto it = std::find_if(m->fonts.begin(), m->fonts.end(), [filepath, size] (auto&& font)
		{
			return (font.filepath == filepath && font.size == size);
		});

		if (it != m->fonts.end())
		{
			return it->handle;
		}

		FT_Face ft_face {};
		FT_Error ft_error {};
		
		ft_error = FT_New_Face(m->ft_library, filepath.data(), 0, &ft_face);
		if (ft_error != 0)
		{
			return FontHandleInvalid;
		}

		ft_error = FT_Set_Char_Size(ft_face, 0, (FT_F26Dot6)size * 64, 0, 0);
		if (ft_error != 0)
		{
			FT_Done_Face(ft_face);
			return FontHandleInvalid;
		}

		const auto handle = FontHandle(m->fonts.size());

		Internal::FontEntry entry;
		entry.handle = handle;
		entry.filepath = filepath;
		entry.size = size;
		entry.ft_face = ft_face;
		entry.height = (float)ft_face->size->metrics.height / 64.0f;
		m->fonts.emplace_back(entry);

		for (char c = 32; c < 127; ++c)
		{
			cache_glyph(handle, c);
		}

		return handle;
	}

	//-----------------------------------------------------------------------------------------------------
	// Instancing
	//-----------------------------------------------------------------------------------------------------

	void TextSystem::add_instance(FontHandle handle, std::string_view text, const TextInstanceParameters& parameters)
	{
		const auto font_it = std::find_if(m->fonts.begin(), m->fonts.end(), [handle] (auto&& entry)
		{
			return (entry.handle == handle);
		});

		if (font_it == m->fonts.end())
		{
			return;
		}

		Vector2F offset;

		for (size_t i = 0; i < text.size(); ++i)
		{
			const char c = text[i];

			if (c == ' ' && parameters.wrap_size > 0.0f)
			{
				float length = 0.0f;

				for (size_t j = i + 1; j < text.size() && text[j] != ' '; ++j)
				{
					const char c2 = text[j];

					cache_glyph(font_it->handle, c2);
					const auto sprite_it = font_it->sprite_map.find(c2);

					length += sprite_it->second.advance;
				}

				if (offset.x + length >= parameters.wrap_size)
				{
					offset.x = 0.0f;
					offset.y += font_it->height;

					continue;
				}
			}

			if (c == '\n')
			{
				offset.x = 0.0f;
				offset.y += font_it->height;

				continue;
			}

			cache_glyph(font_it->handle, c);
			const auto sprite_it = font_it->sprite_map.find(c);

			if (sprite_it->second.handle != SpriteHandleInvalid)
			{
				auto color = Vector4F{1.0f, 1.0f, 1.0f, 1.0f};

				if (!parameters.colors.empty())
				{
					color = (i < parameters.colors.size() ? parameters.colors[i] : parameters.colors.back());
				}

				SpriteInstanceParameters sprite_parameters;
				sprite_parameters.position.x = parameters.position.x + offset.x;
				sprite_parameters.position.y = parameters.position.y + offset.y;
				sprite_parameters.position += sprite_it->second.offset;
				sprite_parameters.size = sprite_it->second.size;
				sprite_parameters.anchor = SpriteAnchor::TopLeft;
				sprite_parameters.space = parameters.space;
				sprite_parameters.blend_color = color;
				sprite_parameters.is_grayscale = true;
				sprite_parameters.z_order = parameters.z_order;
				m->engine.sprite().add_instance(sprite_it->second.handle, sprite_parameters);
			}

			offset.x += sprite_it->second.advance;
		}
	}

	//-----------------------------------------------------------------------------------------------------
	// EngineHandler Interface
	//-----------------------------------------------------------------------------------------------------

	void TextSystem::on_deinit(Context& /*context*/)
	{
		m->fonts.clear();
	}

	//-----------------------------------------------------------------------------------------------------
	// Helpers
	//-----------------------------------------------------------------------------------------------------

	bool TextSystem::cache_glyph(FontHandle handle, char glyph)
	{
		FT_Error ft_error {};

		const auto font_it = std::find_if(m->fonts.begin(), m->fonts.end(), [handle] (auto&& entry)
		{
			return (entry.handle == handle);
		});

		const auto sprite_it = font_it->sprite_map.find(glyph);
		if (sprite_it != font_it->sprite_map.end())
		{
			return true;
		}

		const auto glyph_index = FT_Get_Char_Index(font_it->ft_face, glyph);

		ft_error = FT_Load_Glyph(font_it->ft_face, glyph_index, FT_LOAD_DEFAULT);
		if (ft_error != 0)
		{
			return false;
		}

		ft_error = FT_Render_Glyph(font_it->ft_face->glyph, FT_RENDER_MODE_NORMAL);
		if (ft_error != 0)
		{
			return false;
		}

		static const std::map<FT_Pixel_Mode, PixelFormat> s_format_map
		{
			{ FT_PIXEL_MODE_GRAY, PixelFormat::R8UNorm }
		};

		auto sprite_handle = SpriteHandleInvalid;

		if (font_it->ft_face->glyph->bitmap.buffer != nullptr)
		{
			const auto format_it = s_format_map.find((FT_Pixel_Mode)font_it->ft_face->glyph->bitmap.pixel_mode);
			if (format_it == s_format_map.end())
			{
				return false;
			}

			sprite_handle = m->engine.sprite().register_sprite(font_it->ft_face->glyph->bitmap.buffer, font_it->ft_face->glyph->bitmap.pitch, font_it->ft_face->glyph->bitmap.rows, format_it->second);
			if (sprite_handle == SpriteHandleInvalid)
			{
				return false;
			}
		}

		Internal::SpriteEntry sprite_entry;
		sprite_entry.handle = sprite_handle;
		sprite_entry.size.x = (float)font_it->ft_face->glyph->metrics.width / 64.0f;
		sprite_entry.size.y = (float)font_it->ft_face->glyph->metrics.height / 64.0f;
		sprite_entry.offset.x = (float)font_it->ft_face->glyph->bitmap_left;
		sprite_entry.offset.y = (float)-font_it->ft_face->glyph->bitmap_top;
		sprite_entry.advance = (float)font_it->ft_face->glyph->advance.x / 64.0f;

		font_it->sprite_map.insert(std::make_pair(glyph, sprite_entry));

		return true;
	}
}