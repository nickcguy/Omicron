

local mtlManager = Omicron.MaterialManager:GetMaterialManager();
local mtl = mtlManager:GetMaterialBase("Oak Floor");
local uniform = "UVScale"
local scale = mtl:GetUniform(uniform);
mtl:Interp(uniform, scale + 100, 10);