--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

return function(dev, scene, tag)
    local getImagePath = function(fileName) return getAssetPath("skyboxes/deep-space/") .. fileName end
    local tex = sl.Texture.loadCubeFromFiles(dev, {
        getImagePath("Front.png"), getImagePath("Back.png"), getImagePath("Left.png"),
        getImagePath("Right.png"), getImagePath("Top.png"), getImagePath("Bottom.png")
    })
    tex:setWrapping(sl.TextureWrapping.Clamp)
    tex:setFiltering(sl.TextureFiltering.Linear)
    local node = scene:createNode()
    local renderer = node:addComponent("SkyboxRenderer")
    renderer:setTexture(tex)
    renderer:setTag(tag)
end