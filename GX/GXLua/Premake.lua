----------------------------------------------------------------------------------
-- Namespace
----------------------------------------------------------------------------------

GX = GX or {}
GX.Premake = GX.Premake or {}

----------------------------------------------------------------------------------
-- Includes
----------------------------------------------------------------------------------

include "../GXThirdParty/premake-qt/qt.lua"

----------------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------------

GX.Premake.Config = {
	Debug = "Debug",
	Release = "Release"
}

GX.Premake.Link = {
	Dynamic = "Dynamic",
	Static = "Static"
}

GX.Premake.Kind = {
	DLL = {
		Name = "DLL",
		Kind = "SharedLib",
		Link = GX.Premake.Link.Dynamic
	},
	Static = {
		Name = "Static",
		Kind = "StaticLib",
		Link = GX.Premake.Link.Static
	},
	Exe = {
		Name = "Exe",
		Kind = "ConsoleApp",
		Link = GX.Premake.Link.Dynamic
	},
	ExeStatic = {
		Name = "ExeStatic",
		Kind = "ConsoleApp",
		Link = GX.Premake.Link.Static
	},	
	ExeWin = {
		Name = "ExeWin",
		Kind = "WindowedApp",
		Link = GX.Premake.Link.Dynamic
	},
	ExeWinStatic = {
		Name = "ExeWinStatic",
		Kind = "WindowedApp",
		Link = GX.Premake.Link.Static
	}
}

GX.Premake.Platform = {
	X32 = "x32",
	X64 = "x64"
}

----------------------------------------------------------------------------------
-- Functions
----------------------------------------------------------------------------------

function GX.Premake.import(name, base_dir, inc_dir, lib_dir, link)
	includedirs { base_dir .. "/" .. inc_dir }
	libdirs { base_dir .. "/" .. lib_dir }
	links { name }
	
	for config_k, config_v in pairs(GX.Premake.Config) do
		for kind_k, kind_v in pairs(GX.Premake.Kind) do
			if (kind_v.Link == GX.Premake.Link.Dynamic) then
				filter ("configurations:" .. config_v .. kind_v.Name)
					defines { name .. "_IMPORT_DLL" }
				filter{}
			end
		end
	end
end

----------------------------------------------------------------------------------

function GX.Premake.import_qt(qt_path, qt_modules)
	local qt = premake.extensions.qt
	
	qt.enable();
	qtpath (qt_path)
	qtmodules (qt_modules)
	qtprefix "Qt5"
	
	files {
		"../Source/**.ui",
		"../Source/**.qrc"
	}		
	
	vpaths {
		["Source/*"] = {
			"../Source/**.ui",
			"../Source/**.qrc"
		}
	}	
	
	filter ("configurations:*" .. GX.Premake.Config.Debug .. "*")
		qtsuffix "d"
	filter {}
end

----------------------------------------------------------------------------------

function GX.Premake.import_gx(gx_path, gx_modules)
	for _,name in pairs(gx_modules) do
		GX.Premake.import(name, gx_path .."/" .. name, "Include", "Binaries")
	end
end

----------------------------------------------------------------------------------

function GX.Premake.workspace(kinds)
	local configurations_t = {}
	for _,c in pairs(GX.Premake.Config) do
		for _,k in pairs(kinds) do
			table.insert(configurations_t, c .. k.Name)
		end
	end
	
	local platforms_t = {}
	for _,p in pairs(GX.Premake.Platform) do
		table.insert(platforms_t, p)
	end	

	configurations (configurations_t)
	platforms (platforms_t)
	
	filter "action:vs*"
		location "MSVC/"
		
	filter {}
end

----------------------------------------------------------------------------------

function GX.Premake.project(name, kinds)
	local configurations_t = {}
	for config_k, config_v in pairs(GX.Premake.Config) do
		for kind_k, kind_v in pairs(kinds) do
			table.insert(configurations_t, config_v .. kind_v.Name)
		end
	end

	local configmap_t = {}
	for config_k, config_v in pairs(GX.Premake.Config) do
		for kind_k, kind_v in pairs(GX.Premake.Kind) do
			local found = false
			for kind_local_k, kind_local_v in pairs(kinds) do
				if (kind_local_v.Link == kind_v.Link) then
					configmap_t[config_v .. kind_v.Name] = config_v .. kind_local_v.Name
					found = true
					break
				end
			end
			if (found == false) then
				configmap_t[config_v .. kind_v.Name] = config_v .. kinds[1].Name
			end
		end
	end

	local platforms_t = {}
	for _,p in pairs(GX.Premake.Platform) do
		table.insert(platforms_t, p)
	end

	configurations (configurations_t)
	configmap (configmap_t)
	platforms (platforms_t)
	targetname (name)
	targetdir "../Bin"
	debugdir "../Bin"
	language "C++"
	cppdialect "C++17"
	warnings "Extra"
	flags { "FatalWarnings" }
	
	disablewarnings {
		"4251", -- class 'X' needs to have dll-interface to be used by clients of class 'Y'
		"4275", -- non dll-interface class 'X' used as base for dll-interface class 'Y'
	}

	for config_k, config_v in pairs(GX.Premake.Config) do
		for kind_k, kind_v in pairs(GX.Premake.Kind) do
			filter ("configurations:" .. config_v .. kind_v.Name)
				kind (kind_v.Kind)
		end
	end
	
	filter ("configurations:*" .. GX.Premake.Config.Release .. "*")
		defines { "NDEBUG" }
		optimize "Speed"
	
	filter ("configurations:*" .. GX.Premake.Kind.DLL.Name)
		defines { name .. "_EXPORT_DLL" }
		postbuildcommands {
			"{MKDIR} %{wks.location}DLLOutput",
			"{COPY} %{cfg.buildtarget.directory}%{cfg.buildtarget.basename}.dll %{wks.location}DLLOutput",
			"{COPY} %{cfg.buildtarget.directory}%{cfg.buildtarget.basename}.pdb %{wks.location}DLLOutput"
		}
		
	filter ("configurations:*Exe*")
		postbuildcommands {
			"{MKDIR} %{wks.location}DLLOutput",
			"{COPY} %{wks.location}DLLOutput/* %{cfg.buildtarget.directory}"
		}
	
	filter ("platforms:" .. GX.Premake.Platform.X32)
		architecture "x32"
	
	filter ("platforms:" .. GX.Premake.Platform.X64)
		architecture "x64"
	
	filter "action:vs*"
		location "MSVC/"
		objdir "$(ProjectDir)Intermediate/"
		
	for _,c in pairs(GX.Premake.Config) do
		for _,p in pairs(GX.Premake.Platform) do
			for _,k in pairs(kinds) do
				filter { "configurations:" .. c .. k.Name, "platforms:" .. p }
					targetsuffix ("." .. c .. "." .. p)
			end
		end
	end
		
	filter {}
		
	files {
		"../Source/**.cpp",
		"../Source/**.h",
		"../Source/**.inl",
		"../Resource/**.qrc"
	}		
	
	vpaths {
		["Source/*"] = {
			"../Source/**.cpp",
			"../Source/**.h",
			"../Source/**.inl",
		},
		["Resource/*"] = {
			"../Resource/**.qrc"
		}
	}
	
	includedirs {
		"../Include"
	}
end

----------------------------------------------------------------------------------

function GX.Premake.enable_pch(source, header)
	filter "action:vs*"
		pchheader (header)
		pchsource (source)
		buildoptions { "/FI\"" .. header .. "\"" }
	filter {}
end