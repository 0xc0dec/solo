--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local tags = require 'tags'
local assetCache = require 'asset-cache'

local function createFont()
    local textureWidth = 1024
    local textureHeight = 1024
    local lineHeight = 60
    local firstChar = string.byte(' ')
    local charCount = string.byte('~') - string.byte(' ')
    local path = assetPath('fonts/aller.ttf')
    local font = sl.Font.loadFromFile(sl.device, path, lineHeight, textureWidth, textureHeight, firstChar, charCount, 2, 2)

    local mesh = sl.FontMesh.fromFont(sl.device, font)

    local effect = assetCache.effect('font')
    local material = sl.Material.fromEffect(sl.device, effect)
    material:setFaceCull(sl.FaceCull.None)
    material:setBlend(true)
    material:setDepthTest(true)
    material:setDepthWrite(false)
    material:bindParameter('matrices:wvp', sl.ParameterBinding.WorldViewProjectionMatrix)
    material:setTextureParameter('mainTex', font:atlas())
    
    return {
        font = font,
        mesh = mesh,
        material = material
    }
end

return function(text, parentNode, offset, rotationAngle)
    local font = createFont()
    font.mesh:setText(text)

    local textNode = parentNode:scene():createNode()
    local textTransform = textNode:findComponent('Transform')
    textTransform:setLocalScale(vec3(0.01, 0.01, 1))

    local renderer = textNode:addComponent('MeshRenderer')
    renderer:setMaterial(0, font.material)
    renderer:setTag(tags.transparent)
    renderer:setMesh(font.mesh:mesh())
    
    local parentPos = parentNode:findComponent('Transform'):worldPosition()
    textTransform:setWorldPosition(parentPos + offset)
    textTransform:rotateByAxisAngle(vec3(0, 1, 0), rotationAngle, sl.TransformSpace.World)

    return {
        node = textNode
    }
end