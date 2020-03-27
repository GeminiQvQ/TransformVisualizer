// Local Includes
#include <GXGfx/System/Text/TextTypes.h>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	TextInstanceParameters::TextInstanceParameters()
		: position {}
		, colors {}
		, space {SpriteSpace::World}
		, wrap_size {}
		, z_order {}
	{
	}
}