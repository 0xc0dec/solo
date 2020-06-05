--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function()
    return sl.createComponent('Grabber', {
        init = function(self)
            self.tracer = self.node:findScriptComponent(sl.cmpId('Tracer'))
            self.transform = self.node:findComponent('Transform')
            self.node = nil
        end,

        update = function(self)
            if sl.device:isKeyPressed(sl.KeyCode.G, true) then
                if self.node then
                    self.node:findComponent('Transform'):setParent(nil)
                    self.node = nil
                elseif self.tracer.hitNode then
                    self.node = self.tracer.hitNode
                    self.node:findComponent('Transform'):setParent(self.transform)
                end
            end
        end
    })
end