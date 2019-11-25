#pragma once

// Project Includes
#include <TransformVisualizer/API.h>

namespace TransformVisualizer
{
	class Kernel
	{
	public:
		// Construction & Destruction
				Kernel	();
				~Kernel	();

		// Initialization
		void	create	();
		void	destroy	();

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}