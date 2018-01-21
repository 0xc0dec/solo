--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

local createRotator = require "Rotator"
local layers = require "Layers"

return function(scene, assetCache)
    local tex = sl.Texture2d.loadFromFile(sl.device, getAssetPath("textures/Flammable.png"), true)
    tex:setAnisotropyLevel(16)
    tex:setWrap(sl.TextureWrap.ClampToEdge)

    local material = sl.Material.create(sl.device, assetCache.getEffect("Texture"))
    material:setFaceCull(sl.FaceCull.None)
    material:setBlend(true)
    material:setDepthTest(true)
    material:setDepthWrite(false)
    material:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setTextureParameter("mainTex", tex)

    local parent = scene:createNode()
    parent:findTransform():setLocalPosition(vec3(5, 0, 0))
    parent:addScriptComponent(createRotator("world", vec3(0, 1, 0), 1))

    local node = scene:createNode()
    node:addScriptComponent(createRotator("local", vec3(1, 0, 0), 1))
    node:findTransform():setParent(parent:findTransform())
    node:findTransform():setLocalPosition(vec3(2, 0, 0))

    local renderer = node:addMeshRenderer()
    renderer:setMesh(assetCache.meshes.quad)
    renderer:setMaterial(0, material)
    renderer:setLayer(layers.transparent)

    return parent
end