// Local Includes
#include <GXLib/Core/EnableTypeID.h>

namespace GX
{
	int EnableTypeID::get_type_id() const
	{
		return -1;
	}

	std::vector<int> EnableTypeID::get_type_ids() const
	{
		return std::vector<int>{};
	}

	int EnableTypeID::create_type_id()
	{
		static int s_id = 0;
		return ++s_id;
	}
}