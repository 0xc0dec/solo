--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local layers = require "Layers"

local getImagePath = function(fileName)
    return getAssetPath("textures/skyboxes/deep-space/") .. fileName
end

return function(scene)
    sl.CubeTexture.loadFromFaceFilesAsync(sl.device,
        getImagePath("Front.png"), getImagePath("Back.png"),
        getImagePath("Left.png"), getImagePath("Right.png"),
        getImagePath("Top.png"), getImagePath("Bottom.png")
    ):done(function(tex)
        tex:setWrap(sl.TextureWrap.ClampToEdge)

        local node = scene:createNode()
        local renderer = node:addSkyboxRenderer()
        renderer:setTexture(tex)
        renderer:setLayer(layers.skybox)
    end)
end