--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

local createRotator = require "Rotator"
local tags = require "Tags"

return function(scene, assetCache)
    local tex = sl.Texture2D.loadFromFile(sl.device, getAssetPath("textures/Flammable.png"), true)
    tex:setAnisotropyLevel(16)
    tex:setWrap(sl.TextureWrap.ClampToEdge)

    local material = sl.Material.create(sl.device, assetCache.getEffect("Texture"))
    material:setFaceCull(sl.FaceCull.None)
    material:setBlend(true)
    material:setDepthTest(true)
    material:setDepthWrite(false)
    material:bindParameter("matrices:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setTextureParameter("mainTex", tex)

    local parent = scene:createNode()
    parent:findComponent("Transform"):setLocalPosition(vec3(5, 0, 0))
    parent:addScriptComponent(createRotator("world", vec3(0, 1, 0), 1))

    local node = scene:createNode()
    node:addScriptComponent(createRotator("local", vec3(1, 0, 0), 1))
    node:findComponent("Transform"):setParent(parent:findComponent("Transform"))
    node:findComponent("Transform"):setLocalPosition(vec3(2, 0, 0))

    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(assetCache.meshes.getQuad())
    renderer:setMaterial(0, material)
    renderer:setTag(tags.transparent)

    return parent
end