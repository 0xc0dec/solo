--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(path)
    return sl.createComponent("Screenshoter", {

        update = function()
            if sl.device:isKeyPressed(sl.KeyCode.P, true) then
                sl.device:saveScreenshot(path)
            end
        end
    })
end