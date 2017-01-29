return function(physics)
    return {
        typeId = 8,

        init = function(self)
            self.transform = self.node:findComponent("Transform")
        end,

        update = function(self)
            local worldPos = self.transform:getWorldPosition()
            local localFwd = self.transform:getLocalForward()
            local to = solo.Vector3(
                    worldPos.x + localFwd.x * 100,
                    worldPos.y + localFwd.y * 100,
                    worldPos.z + localFwd.z * 100)

            local hitResults = physics:castRayAll(self.transform:getWorldPosition(), to)

            for i, hit in ipairs(hitResults) do
                local obj = hit.body:getNode():findScriptComponent(6) -- TODO remove ugly hack
                if obj then
                    obj:setActive()
                end
            end
        end
    }
end