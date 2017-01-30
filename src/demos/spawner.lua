local createSpawnedObject = dofile("../../src/demos/spawned-object.lua")

return function(device, mesh, effect)
    return {
        typeId = solo.getCmpId("Spawner"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
            self.scene = self.node:getScene()
        end,

        update = function(self)
            if device:isKeyPressed(solo.KeyCode.Space, true) then
                local initialPos = self.transform:getLocalPosition() + self.transform:getLocalForward() * 2
                local initialRotation = self.transform:getLocalRotation()
                self.scene:createNode():addScriptComponent(
                    createSpawnedObject(device, effect, mesh, initialPos, initialRotation))
            end
        end
    }
end