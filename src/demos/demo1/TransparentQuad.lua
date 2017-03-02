--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "Rotator"

return function(dev, scene, effects, quadMesh, tag, tex)
    tex:setWrapping(solo.TextureWrapping.Clamp)

    local material = solo.Material.create(dev, effects.simpleTexture)
    material:setFaceCull(solo.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:setTextureParameter("mainTex", tex)
    material:setBlend(true)
    material:setDepthTest(true)
    material:setDepthWrite(false)

    local parent = scene:createNode()
    parent:findComponent("Transform"):setLocalPosition(vec3(5, 0, 0))
    parent:addScriptComponent(createRotator(dev, "world", vec3(0, 1, 0)))

    local node = scene:createNode()
    node:addScriptComponent(createRotator(dev, "local", vec3(1, 0, 0)))
    node:findComponent("Transform"):setParent(parent:findComponent("Transform"))
    node:findComponent("Transform"):setLocalPosition(vec3(2, 0, 0))

    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(quadMesh)
    renderer:setMaterial(0, material)
    renderer:setTag(tag)

    return parent
end