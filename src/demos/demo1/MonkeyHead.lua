--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "Rotator"

return function(scene, effects, tex, mesh)
    local material = sl.Material.create(sl.device, effects.basicLighting)
    material:setFaceCull(sl.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:bindInvTransposedWorldMatrixParameter("invTransposedWorldMatrix")
    material:setTextureParameter("mainTex", tex)

    local node = scene:createNode()
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(mesh)
    renderer:setMaterial(0, material)
    node:findComponent("Transform"):setLocalPosition(vec3(0, 0, 0))
    node:addScriptComponent(createRotator("local", vec3(1, 0, 0)))
end