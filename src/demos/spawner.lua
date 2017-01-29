local createSpawnedObject = dofile("../../src/demos/spawned-object.lua")

return function(device, mesh, effect)
    return {
        typeId = 7,

        init = function(self)
            self.transform = self.node:findComponent("Transform")
            self.scene = self.node:getScene()
        end,

        update = function(self)
            if device:isKeyPressed(solo.KeyCode.Space, true) then
                local localPos = self.transform:getLocalPosition()
                local localForward = self.transform:getLocalForward()
                local initialPos = solo.Vector3(
                    localPos.x + localForward.x * 2,
                    localPos.y + localForward.y * 2,
                    localPos.z + localForward.z * 2)
                local initialRotation = self.transform:getLocalRotation()
                self.scene:createNode():addScriptComponent(
                    createSpawnedObject(device, effect, mesh, initialPos, initialRotation))
            end
        end
    }
end