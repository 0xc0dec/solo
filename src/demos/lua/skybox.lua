--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local tags = require "tags"

local getImagePath = function(fileName)
    return getAssetPath("textures/skyboxes/deep-space/") .. fileName
end

return function(scene, assetCache)
    local mesh = assetCache.meshes.getQuad()
    local tag = tags.skybox
    
    local node = scene:createNode()

    local effect = assetCache.getEffect("skybox")
    local material = sl.Material.create(sl.device, effect)
    material:setDepthTest(true)
    material:setDepthWrite(false)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices:proj", sl.ParameterBinding.ProjectionMatrix)
    material:bindParameter("matrices:worldView", sl.ParameterBinding.WorldViewMatrix)
    
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(mesh)
    renderer:setTag(tag)
    renderer:setMaterial(0, material)

    sl.CubeTexture.loadFromFaceFilesAsync(sl.device,
        getImagePath("+x.png"), getImagePath("-x.png"),
        getImagePath("+y.png"), getImagePath("-y.png"),
        getImagePath("+z.png"), getImagePath("-z.png")
    ):done(function(tex)
        tex:setWrap(sl.TextureWrap.ClampToEdge)
        material:setTextureParameter("colorMap", tex)
    end)

    return {
        node = node,
        renderer = renderer,
        tag = tag
    }
end