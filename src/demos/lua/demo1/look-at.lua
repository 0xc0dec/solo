--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require 'common'

return function(getTargetPos)
    return sl.createComponent('LookAt', {
        init = function(self)
            self.transform = self.node:findComponent('Transform')
        end,

        update = function(self)
            self.transform:lookAt(getTargetPos(), vec3(0, 1, 0))
        end
    })
end