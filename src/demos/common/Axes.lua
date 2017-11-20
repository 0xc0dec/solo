--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(dev, assetCache)
    local mesh = sl.Mesh.loadFromFile(dev, getAssetPath("meshes/Axes.obj"))
    local effect = assetCache.getEffect("Color")

    local createColorMaterial = function(color)
        local mat = sl.Material.create(dev, effect)
        mat:setFaceCull(sl.FaceCull.All)

        if dev:getMode() == sl.DeviceMode.Vulkan then
            mat:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
            mat:setVector4Parameter("variables.color", color)
        elseif dev:getMode() == sl.DeviceMode.OpenGL then
            mat:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
            mat:setVector4Parameter("color", color)
        end

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
