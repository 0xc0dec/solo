--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = dofile("../../src/demos/common/Rotator.lua")
local createLookAt = dofile("../../src/demos/common/LookAt.lua")

return function(dev, scene, effects, offscreenCameraTex, quadMesh, tag)
	local canvasSize = dev:getCanvasSize()

    local material = solo.Material.create(dev, effects.simpleTexture)
    material:setFaceCull(solo.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:setTextureParameter("mainTex", offscreenCameraTex)

    local parent = scene:createNode()
    parent:findComponent("Transform"):setLocalPosition(vec3(-2, 2, -2))
    parent:addScriptComponent(createRotator(dev, "world", vec3(0, 1, 0)))

    local node = scene:createNode()
    node:addScriptComponent(createLookAt(vec3(0, 0, 0)))

    local transform = node:findComponent("Transform")
    transform:setParent(parent:findComponent("Transform"))
    transform:setLocalPosition(vec3(5, 2, -5))
    transform:setLocalScale(vec3(5, 5 * canvasSize.y / canvasSize.x, 1))
    
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(quadMesh)
    renderer:setMaterial(0, material)
    renderer:setTag(tag)

    return parent
end