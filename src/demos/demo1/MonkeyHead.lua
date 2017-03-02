--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "Rotator"

return function(dev, scene, effects, tex, mesh)
    local material = solo.Material.create(dev, effects.basicLighting)
    material:setFaceCull(solo.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:bindInvTransposedWorldMatrixParameter("invTransposedWorldMatrix")
    material:setTextureParameter("mainTex", tex)

    local node = scene:createNode()
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(mesh)
    renderer:setMaterial(0, material)
    node:findComponent("Transform"):setLocalPosition(vec3(0, 0, 0))
    node:addScriptComponent(createRotator(dev, "local", vec3(1, 0, 0)))
end