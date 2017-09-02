--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

return function(targetPos)
    return {
        typeId = sl.getCmpId("LookAt"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
        end,

        update = function(self)
            self.transform:lookAt(targetPos, vec3(0, 1, 0))
        end
    }
end