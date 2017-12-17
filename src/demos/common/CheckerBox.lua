--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "Rotator"

return function(scene, assetCache, cubeMesh)
    local effect = assetCache.getEffect("Checker")
    local material = sl.Material.create(sl.device, effect)
    material:setFaceCull(sl.FaceCull.None)
    
    if sl.device:getMode() == sl.DeviceMode.Vulkan then
        material:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
        material:setVector4Parameter("variables.color", vec4(1, 1, 0, 1))
    elseif sl.device:getMode() == sl.DeviceMode.OpenGL then
        material:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
        material:setVector4Parameter("color", vec4(1, 1, 0, 1))
    end

    local node = scene:createNode()
    node:addComponent("MeshRenderer"):setMesh(cubeMesh)
    node:findComponent("MeshRenderer"):setMaterial(0, material)
    node:findComponent("Transform"):setLocalPosition(vec3(-5, 0, 0))
    node:addScriptComponent(createRotator("world", vec3(0, 1, 0), 2))
end