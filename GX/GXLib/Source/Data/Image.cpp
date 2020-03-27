// Local Includes
#include <GXLib/Data/Image.h>

// Project Includes
#include <GXLib/Graphics/API/Windows/WindowsPixelFormat.h>

// Third-Party Includes
#include <DirectXTex.h>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Utility
	//-----------------------------------------------------------------------------------------------------

	struct ComWrapper
	{
		ComWrapper()  { CoInitialize(0);  }
		~ComWrapper() { CoUninitialize(); }
	};
}

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct Image::Internal
	{
		DirectX::TexMetadata	metadata;
		DirectX::ScratchImage	scratch_image;

		Internal()
			: metadata {}
			, scratch_image {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	Image::Image()
		: m {std::make_unique<Internal>()}
	{
	}

	Image::~Image()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Load & Save
	//-----------------------------------------------------------------------------------------------------

	bool Image::load(std::string_view filepath)
	{
		ComWrapper com;
		const auto result = DirectX::LoadFromWICFile(std::wstring(filepath.begin(), filepath.end()).c_str(), DirectX::WIC_FLAGS_NONE, &m->metadata, m->scratch_image);
		return SUCCEEDED(result);
	}

	bool Image::save(std::string_view filepath)
	{
		// TODO
		filepath;
		return false;
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	size_t Image::width() const
	{
		return m->metadata.width;
	}

	size_t Image::height() const
	{
		return m->metadata.height;
	}

	GX::PixelFormat Image::format() const
	{
		return from_dxgi_format(m->metadata.format);
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	uint8_t* Image::pixels()
	{
		return m->scratch_image.GetPixels();
	}

	const uint8_t* Image::pixels() const
	{
		return m->scratch_image.GetPixels();
	}
}