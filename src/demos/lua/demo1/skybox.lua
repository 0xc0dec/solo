--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local tags = require "tags"

local texturePath = function(fileName)
    return assetPath("textures/skyboxes/deep-space/") .. fileName
end

return function(scene, assetCache)
    local mesh = assetCache.meshes.quad()
    local tag = tags.skybox
    
    local node = scene:createNode()

    local effect = assetCache.effect("skybox")
    local material = sl.Material.fromEffect(sl.device, effect)
    material:setDepthTest(true)
    material:setDepthWrite(false)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices:proj", sl.ParameterBinding.ProjectionMatrix)
    material:bindParameter("matrices:worldView", sl.ParameterBinding.WorldViewMatrix)
    
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(mesh)
    renderer:setTag(tag)
    renderer:setMaterial(0, material)

    sl.CubeTexture.fromFaceFilesAsync(sl.device,
        texturePath("+x.png"), texturePath("-x.png"),
        texturePath("+y.png"), texturePath("-y.png"),
        texturePath("+z.png"), texturePath("-z.png")
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