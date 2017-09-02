--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(path)
    return {
        typeId = sl.getCmpId("Screenshoter"),

        update = function()
            if sl.device:isKeyPressed(sl.KeyCode.P, true) then
                sl.device:saveScreenshot(path)
            end
        end
    }
end