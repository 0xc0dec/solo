return function(device, fs)
    local textureWidth = 1024
    local textureHeight = 1024
    local lineHeight = 60
    local renderer

    return {
        typeId = solo.getCmpId("TimeLabelUpdater"),

        init = function(self)
            local fontData = fs:readBytes(getAssetPath("aller.ttf"))
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