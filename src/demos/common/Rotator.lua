--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(device, space, axis)
    return {
        typeId = sl.getCmpId("Rotator"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
        end,

        update = function(self)
            local angle = device:getTimeDelta()
            local rotationSpace = sl.TransformSpace.World
            if space == "local" then
                rotationSpace = sl.TransformSpace.Self
            end
            self.transform:rotateByAxisAngle(axis, sl.Radian(angle), rotationSpace)
        end
    }
end