--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(scene, loader, getAssetPath, tag)
    local getImagePath = function(fileName) return getAssetPath("skyboxes/deep-space/") .. fileName end
    loader:loadCubeTextureAsync({
        getImagePath("Front.png"), getImagePath("Back.png"), getImagePath("Left.png"),
        getImagePath("Right.png"), getImagePath("Top.png"), getImagePath("Bottom.png")
    }):done(function(tex)
        tex:setWrapping(solo.TextureWrapping.Clamp)
        tex:setFiltering(solo.TextureFiltering.Linear)
        local node = scene:createNode()
        local renderer = node:addComponent("SkyboxRenderer")
        renderer:setTexture(tex)
        renderer:setTag(tag)
    end)
end