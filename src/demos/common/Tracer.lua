--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(physics)
    return sl.createComponent("Tracer", {
        init = function(self)
            self.transform = self.node:findComponent("Transform")
            self.scene = self.node:getScene()
            self.camera = self.node:findComponent("Camera")
        end,

        update = function(self)
            local mousePos = sl.device:getMousePosition()
            local ray = self.camera:windowPointToWorldRay(mousePos)
            local from = ray:getOrigin()
            local to = from + ray:getDirection() * 100
            local hitResult = physics:rayTestFirst(from, to)
            if hitResult.body then
                self.hitNode = hitResult.body:getNode()
            else
                self.hitNode = nil
            end
            print(self.hitNode)
        end,
    });
end