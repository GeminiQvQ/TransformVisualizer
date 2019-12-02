#pragma once

// Project Includes
#include <GXQt/API.h>

// Third-Party Includes
#include <QIcon>

namespace GX::QT::RC
{
	enum class Image
	{
		Add,
		Remove,
		Clear,
		Revert,
		Undo,
		Redo,

		FileNew,
		FileOpen,
		FileSave,
		FileSaveAs
	};

	// Resource Access
	GXQT_API QIcon icon(Image image);
}