----------------------------------------------------------------------------------
-- Namespace
----------------------------------------------------------------------------------

GX = GX or {}
GX.String = GX.String or {}

----------------------------------------------------------------------------------
-- Functions
----------------------------------------------------------------------------------

function GX.String.indent_to_string(indent)
	return string.rep("    ", indent)
end

----------------------------------------------------------------------------------

function GX.String.table_to_string(t, indent)
	indent = indent or 0
	if (type(t) == "table") then
		local s = GX.String.indent_to_string(indent) .. "{\n"
		for k,v in pairs(t) do
			if (type(k) ~= "number") then
				k = "\"" .. k .. "\""
			end
			s = s .. GX.String.indent_to_string(indent+1) .. k .. " = " .. GX.String.table_to_string(v, indent+1) .. "\n"
		end
		s = s .. GX.String.indent_to_string(indent) .. "}"
		return s;
	else
		return tostring(t)
	end
end