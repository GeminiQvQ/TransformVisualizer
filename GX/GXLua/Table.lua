----------------------------------------------------------------------------------
-- Namespace
----------------------------------------------------------------------------------

GX = GX or {}
GX.Table = GX.Table or {}

----------------------------------------------------------------------------------
-- Functions
----------------------------------------------------------------------------------

function GX.Table.contains(table, value)
	for k, v in pairs(table) do
		if (v == value) then
			return true
		end
	end
	return false
end