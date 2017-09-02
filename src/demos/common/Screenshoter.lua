--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(device, path)
    return {
        typeId = sl.getCmpId("Screenshoter"),

        update = function()
            if device:isKeyPressed(sl.KeyCode.P, true) then
                device:saveScreenshot(path)
            end
        end
    }
end