--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(assetCache)
    local layout = sl.VertexBufferLayout()
    layout:addAttribute(sl.VertexAttributeUsage.Position)
    local mesh = sl.Mesh.fromFile(sl.device, assetPath("meshes/axes.obj"), layout)
    
    local effect = assetCache.getEffect("color")

    local createColorMaterial = function(color)
        local mat = sl.Material.fromEffect(sl.device, effect)
        mat:setFaceCull(sl.FaceCull.None)
        mat:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
        mat:setVector4Parameter("variables:color", color)

        return mat
    end

    local materials = {
        red = createColorMaterial(vec4(1, 0, 0, 1)),
        green = createColorMaterial(vec4(0, 1, 0, 1)),
        blue = createColorMaterial(vec4(0, 0, 1, 1)),
        white = createColorMaterial(vec4(1, 1, 1, 1))
    }

    return function(node)
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(mesh)
        renderer:setMaterial(0, materials.blue)
        renderer:setMaterial(1, materials.green)
        renderer:setMaterial(2, materials.white)
        renderer:setMaterial(3, materials.red)
    end
end
