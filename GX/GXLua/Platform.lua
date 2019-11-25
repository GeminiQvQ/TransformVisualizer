----------------------------------------------------------------------------------
-- Namespace
----------------------------------------------------------------------------------

GX = GX or {}
GX.Platform = {}

----------------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------------

GX.Platform.PlatformType = {
	Windows = "Windows",
	Unix 	= "Unix",
	Unknown = "Unknown"
}

----------------------------------------------------------------------------------
-- Functions
----------------------------------------------------------------------------------

function GX.Platform.get_platform()
	local s = package.config:sub(1, 1)
	if (s == "/")  then return GX.Platform.PlatformType.Unix 	end
	if (s == "\\") then return GX.Platform.PlatformType.Windows end
						return GX.Platform.PlatformType.Unknown
end