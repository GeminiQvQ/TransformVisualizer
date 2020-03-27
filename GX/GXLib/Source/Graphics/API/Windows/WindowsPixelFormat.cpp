// Local Includes
#include <GXLib/Graphics/API/Windows/WindowsPixelFormat.h>

// Stdlib Includes
#include <algorithm>
#include <map>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// DXGI Format Conversion
	//-----------------------------------------------------------------------------------------------------

	const std::map<PixelFormat, DXGI_FORMAT>& format_map()
	{
		static const std::map<PixelFormat, DXGI_FORMAT> s_map
		{
			{ PixelFormat::R32G32Float,		  DXGI_FORMAT_R32G32_FLOAT			},
			{ PixelFormat::R32G32B32Float,	  DXGI_FORMAT_R32G32B32_FLOAT		},
			{ PixelFormat::R32G32B32A32Float, DXGI_FORMAT_R32G32B32A32_FLOAT	},
			{ PixelFormat::R8UNorm,			  DXGI_FORMAT_R8_UNORM				},
			{ PixelFormat::R8G8B8A8UNorm,	  DXGI_FORMAT_R8G8B8A8_UNORM		},
			{ PixelFormat::R8G8B8A8UNormSRGB, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB	},
			{ PixelFormat::B8G8R8A8Unorm,	  DXGI_FORMAT_B8G8R8A8_UNORM		},
			{ PixelFormat::R32SInt,			  DXGI_FORMAT_R32_SINT				},

			{ PixelFormat::Invalid,			  DXGI_FORMAT_UNKNOWN				}
		};
		return s_map;
	}

	//-----------------------------------------------------------------------------------------------------

	DXGI_FORMAT to_dxgi_format(PixelFormat format)
	{
		const auto it = format_map().find(format);
		return it->second;
	}

	PixelFormat from_dxgi_format(DXGI_FORMAT format)
	{
		const auto it = std::find_if(format_map().begin(), format_map().end(), [format] (auto&& kv)
		{
			return (kv.second == format);
		});
		return (it != format_map().end() ? it->first : PixelFormat::Invalid);
	}
}