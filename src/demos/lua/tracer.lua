--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(physics)
    return sl.createComponent("Tracer", {
        init = function(self)
            self.transform = self.node:findComponent("Transform") 
            self.scene = self.node:scene()
            self.camera = self.node:findComponent("Camera")
        end,

        update = function(self)
            if sl.device:isMouseButtonDown(sl.MouseButton.Right, false) then
                self.hitNode = nil
                return
            end

            local mousePos = sl.device:mousePosition()
            local ray = self.camera:windowPointToWorldRay(mousePos)
            local from = ray:origin()
            local to = from + ray:direction() * 100
            local hitResult = physics:rayTestFirst(from, to)
            if hitResult.body then
                self.hitNode = hitResult.body:node()
            else
                self.hitNode = nil
            end
        end,
    });
end