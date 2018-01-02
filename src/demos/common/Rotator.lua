--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(space, axis, speed)
    return sl.createComponent("Rotator", {

        init = function(self)
            self.transform = self.node:findComponent("Transform")
        end,

        update = function(self)
            local angle = sl.device:getTimeDelta() * speed
            local rotationSpace = sl.TransformSpace.World
            if space == "local" then
                rotationSpace = sl.TransformSpace.Self
            end
            self.transform:rotateByAxisAngle(axis, sl.Radian(angle), rotationSpace)
        end
    })
end