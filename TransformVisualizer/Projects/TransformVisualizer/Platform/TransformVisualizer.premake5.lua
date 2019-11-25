----------------------------------------------------------------------------------
-- Includes
----------------------------------------------------------------------------------

include "../../GX/GXLua/Premake.lua"

----------------------------------------------------------------------------------
-- Project
----------------------------------------------------------------------------------

project "TransformVisualizer"
	GX.Premake.project("TransformVisualizer", {GX.Premake.Kind.ExeWin, GX.Premake.Kind.ExeWinStatic})
	GX.Premake.import_gx("../../../../GX", {"GXGfx", "GXLib", "GXQt"})
	GX.Premake.import_qt("../../../../../Libraries/Qt/5.13.1/msvc2017_64", {"core", "gui", "widgets"})
	GX.Premake.enable_pch("../Source/Core/PCH.cpp", "TransformVisualizer/Core/PCH.h");
	
	disablewarnings {
		"4127", -- conditional expression is constant
		"4458", -- declaration of 'X' hides class member
	}