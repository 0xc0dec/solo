--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createUpdater = function(device, fontData)
    local textureWidth = 1024
    local textureHeight = 1024
    local lineHeight = 60
    local renderer

    return {
        typeId = solo.getCmpId("TimeLabelUpdater"),

        init = function(self)
            local firstChar = string.byte(" ")
            local charCount = string.byte("~") - string.byte(" ")
            local font = solo.Font.create(device, fontData, lineHeight, textureWidth,
                textureHeight, firstChar, charCount, 2, 2)

            renderer = self.node:addComponent("FontRenderer")
            renderer:setFont(font)
        end,

        update = function()
            renderer:setText(os.date("Now is %H:%M:%S"))
        end
    }
end

return function(dev, scene, tag, fontData)
    local node = scene:createNode()
    node:addScriptComponent(createUpdater(dev, fontData))
    node:findComponent("FontRenderer"):setTag(tag)
    node:findComponent("Transform"):setLocalScale(vec3(0.02, 0.02, 1))
    node:findComponent("Transform"):setLocalPosition(vec3(-3, 0, 4))
end