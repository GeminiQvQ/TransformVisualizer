#pragma once

#include <GXQt/API.h>

namespace GX::QT::RenderWidget
{
	class GXQT_API Handler
	{
	public:
		virtual bool on_draw();
	};
}