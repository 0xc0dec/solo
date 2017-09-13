--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

local createRotator = require "Rotator"

return function(dev, scene, effects, mesh, tag)
    local tex = sl.Texture2d.loadFromFile(dev, getAssetPath("textures/Flammable.png"))
    tex:generateMipmaps()
    tex:setFiltering(sl.TextureFiltering.LinearMipmapNearest)
    tex:setAnisotropyLevel(8)
    tex:setWrapping(sl.TextureWrapping.Clamp)

    local material = sl.Material.create(dev, effects.simpleTexture)
    material:setFaceCull(sl.FaceCull.All)
    material:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setTextureParameter("mainTex", tex)
    material:setBlend(true)
    material:setDepthTest(true)
    material:setDepthWrite(false)

    local parent = scene:createNode()
    parent:findComponent("Transform"):setLocalPosition(vec3(5, 0, 0))
    parent:addScriptComponent(createRotator("world", vec3(0, 1, 0)))

    local node = scene:createNode()
    node:addScriptComponent(createRotator("local", vec3(1, 0, 0)))
    node:findComponent("Transform"):setParent(parent:findComponent("Transform"))
    node:findComponent("Transform"):setLocalPosition(vec3(2, 0, 0))

    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(mesh)
    renderer:setMaterial(0, material)
    renderer:setTag(tag)

    return parent
end