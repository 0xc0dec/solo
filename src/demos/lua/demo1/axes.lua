--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require 'rotator'
local createLookAt = require 'look-at'

return function(assetCache, scene)
    function createColorMaterial(color)
        local mat = sl.Material.fromEffect(sl.device, assetCache.effect('color'))
        mat:setFaceCull(sl.FaceCull.None)
        mat:bindParameter('matrices:wvp', sl.ParameterBinding.WorldViewProjectionMatrix)
        mat:setVector4Parameter('variables:color', color)
        return mat
    end

    local materials = {
        red = createColorMaterial(vec4(1, 0, 0, 1)),
        green = createColorMaterial(vec4(0, 1, 0, 1)),
        blue = createColorMaterial(vec4(0, 0, 1, 1)),
        white = createColorMaterial(vec4(1, 1, 1, 1))
    }

    function createAxesNode()
        local node = scene:createNode()
        local renderer = node:addComponent('MeshRenderer')
        renderer:setMesh(assetCache.meshes.axes())
        renderer:setMaterial(0, materials.blue)
        renderer:setMaterial(1, materials.green)
        renderer:setMaterial(2, materials.white)
        renderer:setMaterial(3, materials.red)
        return node
    end

    function createLookAtAxesNode(parentTransform, pos, target)
        local node = createAxesNode()
        local transform = node:findComponent('Transform')
        transform:setParent(parentTransform)
        transform:setLocalPosition(pos)
        node:addScriptComponent(createLookAt(target))
    end

    return function()
        local rootNode = createAxesNode()
        local rootNodeTr = rootNode:findComponent('Transform')
        rootNode:addScriptComponent(createRotator('world', vec3(0, 1, 0), 1))

        createLookAtAxesNode(rootNodeTr, vec3(4, 7, 4), vec3(3, 0, -3))
        createLookAtAxesNode(rootNodeTr, vec3(-4, 7, 4), vec3(-3, 1, 3))
        createLookAtAxesNode(rootNodeTr, vec3(4, 7, -4), vec3(3, 1, 3))
    end
end