--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(effects, node, mesh)
	-- TODO cache materials
	local createColorMaterial = function(color)
	    local mat = sl.Material.create(sl.device, effects.color)
	    mat:setFaceCull(sl.FaceCull.All)
	    mat:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
	    mat:setVector4Parameter("color", color)
	    return mat
	end

	local materials = {
	    red = createColorMaterial(vec4(1, 0, 0, 1)),
	    green = createColorMaterial(vec4(0, 1, 0, 1)),
	    blue = createColorMaterial(vec4(0, 0, 1, 1)),
	    white = createColorMaterial(vec4(1, 1, 1, 1))
	}

	local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(mesh)
    renderer:setMaterial(0, materials.blue)
    renderer:setMaterial(1, materials.green)
    renderer:setMaterial(2, materials.white)
    renderer:setMaterial(3, materials.red)
end
