--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(physics)
    return {
        typeId = solo.getCmpId("SpawnedObjectTargeter"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
        end,

        update = function(self)
            local to = self.transform:getWorldPosition() + self.transform:getLocalForward() * 100
            local hitResults = physics:castRayAll(self.transform:getWorldPosition(), to)

            for i, hit in ipairs(hitResults) do
                local obj = hit.body:getNode():findScriptComponent(solo.getCmpId("SpawnedObject"))
                if obj then
                    obj:setActive()
                end
            end
        end
    }
end