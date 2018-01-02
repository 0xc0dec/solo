--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(scene, layer)
    local font

    local createUpdater = function()
        local renderer

        return sl.createComponent("TimeLabelUpdater", {
            init = function(self)
                renderer = self.node:addComponent("FontRenderer")
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
    node:findComponent("FontRenderer"):setLayer(layer)
    node:findComponent("Transform"):setLocalScale(vec3(0.02, 0.02, 1))
    node:findComponent("Transform"):setLocalPosition(vec3(-3, 0, 4))
end