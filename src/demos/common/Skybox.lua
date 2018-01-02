--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local getImagePath = function(fileName)
    return getAssetPath("textures/skyboxes/deep-space/") .. fileName
end

return function(scene, layer)
    sl.CubeTexture.loadFromFaceFilesAsync(sl.device,
        getImagePath("Front.png"), getImagePath("Back.png"),
        getImagePath("Left.png"), getImagePath("Right.png"),
        getImagePath("Top.png"), getImagePath("Bottom.png")
    ):done(function(tex)
        tex:setWrap(sl.TextureWrap.ClampToEdge)

        local node = scene:createNode()
        local renderer = node:addComponent("SkyboxRenderer")
        renderer:setTexture(tex)
        renderer:setLayer(layer)
    end)
end