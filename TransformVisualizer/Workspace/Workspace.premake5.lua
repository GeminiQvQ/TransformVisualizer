----------------------------------------------------------------------------------
-- Includes
----------------------------------------------------------------------------------

include "../../GX/GXLua/Premake.lua"

----------------------------------------------------------------------------------
-- Workspace
----------------------------------------------------------------------------------

workspace "TransformVisualizer"
	GX.Premake.workspace({GX.Premake.Kind.ExeWinStatic, GX.Premake.Kind.Static, GX.Premake.Kind.ExeWin, GX.Premake.Kind.DLL})
	
	startproject "TransformVisualizer"

	group "GX"
		project "GXGfx"
		project "GXLib"
		project "GXQt"
		
	group ""
		project "TransformVisualizer"
	
dofile("../../GX/GXGfx/Platform/GXGfx.premake5.lua")
dofile("../../GX/GXLib/Platform/GXLib.premake5.lua")
dofile("../../GX/GXQt/Platform/GXQt.premake5.lua")
dofile("../Projects/TransformVisualizer/Platform/TransformVisualizer.premake5.lua")