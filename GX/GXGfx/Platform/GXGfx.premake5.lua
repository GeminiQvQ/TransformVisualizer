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
	GX.Premake.import("FreeType_x64", "../../GXThirdParty/FreeType", "Include", "Bin")
	
	links {
		"d3d11",
		"d3dcompiler",
	}
	
	disablewarnings {
		"4324", -- 'X': structure was padded due to alignment specifier
	}
	
	linkoptions {
		-- Generated by statically linking D3D.
		"-IGNORE:4006" -- "X already defined in Y.obj; second definition ignored"
	}