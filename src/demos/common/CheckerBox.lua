--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "Rotator"

return function(scene, assetCache)
    local effect = assetCache.getEffect("Checker")
    local material = sl.Material.create(sl.device, effect)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setVector4Parameter("variables.color", vec4(1, 1, 0, 1))

    local node = scene:createNode()
    node:addMeshRenderer():setMesh(assetCache.meshes.cube)
    node:findMeshRenderer():setMaterial(0, material)
    node:findTransform():setLocalPosition(vec3(-5, 0, 0))
    node:addScriptComponent(createRotator("world", vec3(0, 1, 0), 2))
end