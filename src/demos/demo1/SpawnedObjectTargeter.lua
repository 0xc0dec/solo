--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(dev)
    local physics = dev:getPhysics()

    return {
        typeId = sl.getCmpId("SpawnedObjectTargeter"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
            self.camera = self.node:findComponent("Camera")
        end,

        update = function(self)
            local mousePos = dev:getMousePosition()
            local ray = self.camera:canvasPointToWorldRay(mousePos)
            local from = ray:getOrigin()
            local to = from + ray:getDirection() * 100
            local hitResults = physics:rayTestAll(from, to)

            for i, hit in ipairs(hitResults) do
                local obj = hit.body:getNode():findScriptComponent(sl.getCmpId("SpawnedObject"))
                if obj then
                    obj:setActive()
                end
            end
        end
    }
end