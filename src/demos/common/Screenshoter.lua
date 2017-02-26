--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(device, path)
    return {
        typeId = solo.getCmpId("Screenshoter"),

        update = function()
            if device:isKeyPressed(solo.KeyCode.P, true) then
                device:saveScreenshot(path)
            end
        end
    }
end