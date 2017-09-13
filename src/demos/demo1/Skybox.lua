--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local getImagePath = function(fileName)
    return getAssetPath("textures/skyboxes/deep-space/") .. fileName
end

return function(dev, scene, tag)
    local tex = sl.CubeTexture.loadFromFaceFiles(dev,
        getImagePath("Front.png"), getImagePath("Back.png"),
        getImagePath("Left.png"), getImagePath("Right.png"),
        getImagePath("Top.png"), getImagePath("Bottom.png")
    )
    tex:setWrapping(sl.TextureWrapping.Clamp)
    tex:setFiltering(sl.TextureFiltering.Linear)

    local node = scene:createNode()
    local renderer = node:addComponent("SkyboxRenderer")
    renderer:setTexture(tex)
    renderer:setTag(tag)
end