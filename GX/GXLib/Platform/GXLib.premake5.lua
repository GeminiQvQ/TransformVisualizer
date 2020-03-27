----------------------------------------------------------------------------------
-- Includes
----------------------------------------------------------------------------------

include "../../GXLua/Premake.lua"

----------------------------------------------------------------------------------
-- Project
----------------------------------------------------------------------------------

project "GXLib"
	GX.Premake.project("GXLib", {GX.Premake.Kind.DLL, GX.Premake.Kind.Static})
	
	GX.Premake.import("irrKlang", "../../GXThirdParty/irrKlang", "Include", "Bin")
	GX.Premake.import("DirectXTex_x64", "../../GXThirdParty/DirectXTex", "Include", "Bin")