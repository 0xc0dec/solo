--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(assetCache)
    local effect = assetCache.getEffect("Color")

    local material = sl.Material.create(sl.device, effect)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setVector4Parameter("variables.color", vec4(0, 1, 1, 1))

    return sl.createComponent("PostProcessorControlPanel", {
        init = function(self)
            local scene = self.node:getScene()
            local node = scene:createNode()
            
            local renderer = node:addComponent("MeshRenderer")
            renderer:setMesh(assetCache.meshes.cube)
            renderer:setMaterial(0, material)
    
            local transform = node:findComponent("Transform")
        end,

        update = function(self)

        end
    })
end