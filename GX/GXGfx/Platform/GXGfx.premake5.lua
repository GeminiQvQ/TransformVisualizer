----------------------------------------------------------------------------------
-- Includes
----------------------------------------------------------------------------------

include "../../GXLua/Premake.lua"

----------------------------------------------------------------------------------
-- Project
----------------------------------------------------------------------------------

project "GXGfx"
	GX.Premake.project("GXGfx", {GX.Premake.Kind.DLL, GX.Premake.Kind.Static})
	GX.Premake.import_gx("../../", {"GXLib"})
	
	links {
		"d3d11",
		"d3dcompiler",
	}
	
	linkoptions {
		-- Generated by statically linking Qt.
		"-IGNORE:4006" -- "X already defined in Y.obj; second definition ignored"
	}