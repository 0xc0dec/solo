--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local tags = require "Tags"

local getImagePath = function(fileName)
    return getAssetPath("textures/skyboxes/deep-space/") .. fileName
end

return function(scene)
    sl.CubeTexture.loadFromFaceFilesAsync(sl.device,
        getImagePath("+X.png"), getImagePath("-X.png"),
        getImagePath("+Y.png"), getImagePath("-Y.png"),
        getImagePath("+Z.png"), getImagePath("-Z.png")
    ):done(function(tex)
        tex:setWrap(sl.TextureWrap.ClampToEdge)

        local node = scene:createNode()
        local renderer = node:addComponent("SkyboxRenderer")
        renderer:setTexture(tex)
        renderer:setTag(tags.skybox)
    end)
end