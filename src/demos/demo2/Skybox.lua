--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local getImagePath = function(fileName)
    return getAssetPath("textures/skyboxes/deep-space/") .. fileName
end

-- TODO this is a copy-paste from demo1 Skybox
return function(scene, tag)
    sl.CubeTexture.loadFromFaceFilesAsync(sl.device,
        getImagePath("Front.png"), getImagePath("Back.png"),
        getImagePath("Left.png"), getImagePath("Right.png"),
        getImagePath("Top.png"), getImagePath("Bottom.png")
    ):done(function(tex)
        tex:setWrapping(sl.TextureWrapping.Clamp)
        tex:setFiltering(sl.TextureFiltering.Linear)

        local node = scene:createNode()
        local renderer = node:addComponent("SkyboxRenderer")
        renderer:setTexture(tex)
        renderer:setTag(tag)
    end)
end