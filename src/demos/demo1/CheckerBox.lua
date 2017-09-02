--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "Rotator"

return function(dev, scene, effects, cubeMesh)
    local material = sl.Material.create(dev, effects.checker)
    material:setFaceCull(sl.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:setVector4Parameter("color", vec4(1, 1, 0, 1))

    local node = scene:createNode()
    node:addComponent("MeshRenderer"):setMesh(cubeMesh)
    node:findComponent("MeshRenderer"):setMaterial(0, material)
    node:findComponent("Transform"):setLocalPosition(vec3(-5, 0, 0))
    node:addScriptComponent(createRotator(dev, "world", vec3(0, 1, 0)))
end