--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local layers = require "Layers"

return function(scene)
    local font

    local createUpdater = function()
        local renderer

        return sl.createComponent("TimeLabelUpdater", {
            init = function(self)
                renderer = self.node:addFontRenderer()
                renderer:setFont(font)
            end,

            update = function()
                renderer:setText(os.date("Now is %H:%M:%S"))
            end
        })
    end

    local textureWidth = 1024
    local textureHeight = 1024
    local lineHeight = 60
    local firstChar = string.byte(" ")
    local charCount = string.byte("~") - string.byte(" ")
    local path = getAssetPath("fonts/Aller.ttf")
    font = sl.Font.loadFromFile(sl.device, path,
        lineHeight, textureWidth,
        textureHeight, firstChar, charCount, 2, 2)

    local node = scene:createNode()
    node:addScriptComponent(createUpdater())
    node:findFontRenderer():setLayer(layers.transparent)
    node:findTransform():setLocalScale(vec3(0.02, 0.02, 1))
    node:findTransform():setLocalPosition(vec3(-3, 0, 4))
end