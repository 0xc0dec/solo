--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "rotator"
local createLookAt = require "look-at"
local tags = require "tags"

return function(scene, assetCache, offscreenCameraTex)
	local canvasSize = sl.device:getCanvasSize()

    local material = sl.Material.create(sl.device, assetCache.getEffect("texture"))
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setTextureParameter("mainTex", offscreenCameraTex)

    local parent = scene:createNode()
    parent:findComponent("Transform"):setLocalPosition(vec3(-2, 2, -2))
    parent:addScriptComponent(createRotator("world", vec3(0, 1, 0), 1))

    local node = scene:createNode()
    node:addScriptComponent(createLookAt(vec3(0, 0, 0)))

    local transform = node:findComponent("Transform")
    transform:setParent(parent:findComponent("Transform"))
    transform:setLocalPosition(vec3(5, 2, -5))
    transform:setLocalScale(vec3(5, 5 * canvasSize.y / canvasSize.x, 1))
    
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(assetCache.meshes.getQuad())
    renderer:setMaterial(0, material)
    renderer:setTag(tags.monitor)

    return parent
end