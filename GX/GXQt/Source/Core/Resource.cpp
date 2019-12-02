// Local Includes
#include <GXQt/Core/Resource.h>

// Project Includes
#include <GXLib/Core/EnumReflection.h>

//-----------------------------------------------------------------------------------------------------
// Enum Reflection
//-----------------------------------------------------------------------------------------------------

namespace GX::QT::RC
{
	struct ImageInfo
	{
		Image		type;
		std::string type_name;
		QString		url;
	};
}

DEF_ENUM_REFLECTION_INFO_LOCAL(GX::QT::RC::Image, GX::QT::RC::ImageInfo)
{
	{ GX::QT::RC::Image::Add,		 "", ":/Images/Add"			},
	{ GX::QT::RC::Image::Remove,	 "", ":/Images/Remove"		},
	{ GX::QT::RC::Image::Clear,		 "", ":/Images/Clear"		},
	{ GX::QT::RC::Image::Revert,	 "", ":/Images/Revert"		},
	{ GX::QT::RC::Image::Undo,		 "", ":/Images/Undo"		},
	{ GX::QT::RC::Image::Redo,		 "", ":/Images/Redo"		},
	{ GX::QT::RC::Image::FileNew,	 "", ":/Images/FileNew"		},
	{ GX::QT::RC::Image::FileOpen,	 "", ":/Images/FileOpen"	},
	{ GX::QT::RC::Image::FileSave,	 "", ":/Images/FileSave"	},
	{ GX::QT::RC::Image::FileSaveAs, "", ":/Images/FileSaveAs"	}
};

namespace GX::QT::RC
{
	//-----------------------------------------------------------------------------------------------------
	// Resource Access
	//-----------------------------------------------------------------------------------------------------

	QIcon icon(Image image)
	{
		return QIcon{EnumReflection<Image>::info(image).url};
	}
}