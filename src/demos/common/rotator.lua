--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(device, space, axis)
    return {
        typeId = solo.getCmpId("Rotator"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
        end,

        update = function(self)
            local angle = device:getTimeDelta()
            local rotationSpace = solo.TransformSpace.World
            if space == "local" then
                rotationSpace = solo.TransformSpace.Self
            end
            self.transform:rotateByAxisAngle(axis, solo.Radian(angle), rotationSpace)
        end
    }
end