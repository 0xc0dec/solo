--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require 'rotator'
local assetCache = require 'asset-cache'

return function(scene)
    local effect = assetCache.effect('checker')
    local material = sl.Material.fromEffect(sl.device, effect)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter('matrices:wvp', sl.ParameterBinding.WorldViewProjectionMatrix)
    material:setVector4Parameter('variables:color', vec4(1, 1, 0, 1))

    local node = scene:createNode()
    
    local renderer = node:addComponent('MeshRenderer')
    renderer:setMesh(assetCache.meshes.box())
    renderer:setMaterial(0, material)

    node:addScriptComponent(createRotator('world', vec3(0, 1, 0), 2))

    local transform = node:findComponent('Transform')

    return {
        node = node,
        transform = transform
    }
end