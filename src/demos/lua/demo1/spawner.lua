--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local assetCache = require 'asset-cache'

return function(material)
    local effect = assetCache.effect('color')

    return sl.createComponent('Spawner', {
        init = function(self)
            self.transform = self.node:findComponent('Transform')
            self.scene = self.node:scene()
            self.camera = self.node:findComponent('Camera')
        end,

        update = function(self)
            if sl.device:isKeyPressed(sl.KeyCode.Space, true) then
                self:spawn()
            end
        end,

        spawn = function(self)
            local initialPos = self.transform:localPosition() + self.transform:localForward() * 3
            local initialRotation = self.transform:localRotation()

            local node = self.scene:createNode()
            
            local renderer = node:addComponent('MeshRenderer')
            renderer:setMesh(assetCache.meshes.box())
            renderer:setMaterial(0, material)

            local transform = node:findComponent('Transform')
            transform:setLocalScale(vec3(1, 1, 1))
            transform:setLocalPosition(initialPos)
            transform:setLocalRotation(initialRotation)
    
            local params = sl.RigidBodyParams()
            params.mass = 50
            params.restitution = 0.5
            params.friction = 0.2
            params.linearDamping = 0.1
            params.angularDamping = 0.1
    
            local rigidBody = node:addComponent('RigidBody', params)
            rigidBody:setCollider(sl.BoxCollider.create(vec3(1, 1, 1)))
        end
    })
end
