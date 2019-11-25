----------------------------------------------------------------------------------
-- Namespace
----------------------------------------------------------------------------------

GX = GX or {}
GX.File = {}

----------------------------------------------------------------------------------
-- Includes
----------------------------------------------------------------------------------

include "Platform.lua"

----------------------------------------------------------------------------------
-- Functions
----------------------------------------------------------------------------------

local function get_files_windows(directory, pattern, recursive)
	local result = {}
	
	local pfiles = io.popen("dir /b /a-d \"" .. directory .. "\\" .. pattern .. "\" 2>nul")
	
	for v in pfiles:lines() do
		result[#result+1] = directory .. "\\" .. v
	end
	
	if (recursive == true) then
		local pdirs = io.popen("dir /b /ad \"" .. directory .. "\"  2>nul")
		
		for v in pdirs:lines() do
			local result_ = get_files_windows(directory .. "\\" .. v, pattern, recursive)
			for k2,v2 in pairs(result_) do
				result[#result+1] = v2
			end
		end
	end

	return result
end

local function get_files_unix(directory, pattern, recursive)
	-- TODO
	return {}
end

function GX.File.get_files(directory, pattern, recursive)
	local platform = GX.Platform.get_platform()
	
	if (platform == GX.Platform.PlatformType.Windows) then
		return get_files_windows(directory, pattern, recursive)
	end
	
	if (platform == GX.Platform.PlatformType.Unix) then
		return get_files_unix(directory, pattern, recursive)
	end
	
	return {}
end