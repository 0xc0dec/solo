--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = dofile("../../src/demos/common/Rotator.lua")

return function(dev, scene, shaders, tex, mesh)
    local effect = solo.Effect.create(dev, shaders.vs.basicLighting, shaders.fs.textureWithLighting)

    local material = solo.Material.create(dev, effect)
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