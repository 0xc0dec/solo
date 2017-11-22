--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createSpawnedObject = require "SpawnedObject"

return function(dev, mesh, assetCache)
    return {
        typeId = sl.getCmpId("Spawner"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
            self.scene = self.node:getScene()
        end,

        update = function(self)
            if dev:isKeyPressed(sl.KeyCode.Space, true) then
                local initialPos = self.transform:getLocalPosition() + self.transform:getLocalForward() * 2
                local initialRotation = self.transform:getLocalRotation()
                self.scene:createNode():addScriptComponent(
                    createSpawnedObject(assetCache, mesh, initialPos, initialRotation))
            end
        end
    }
end