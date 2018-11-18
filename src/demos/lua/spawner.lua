--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(assetCache)
    local effect = assetCache.getEffect("color")

    local material = sl.Material.fromEffect(sl.device, effect)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
    material:setVector4Parameter("variables:color", vec4(1, 1, 0, 1))

    return sl.createComponent("Spawner", {
        init = function(self)
            self.transform = self.node:findComponent("Transform")
            self.scene = self.node:scene()
            self.camera = self.node:findComponent("Camera")
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
            
            local renderer = node:addComponent("MeshRenderer")
            renderer:setMesh(assetCache.meshes.getBox())
            renderer:setMaterial(0, material)

            local size = 0.1 + math.random()
    
            local transform = node:findComponent("Transform")
            transform:setLocalScale(vec3(size, size, size))
            transform:setLocalPosition(initialPos)
            transform:setLocalRotation(initialRotation)
    
            local params = sl.RigidBodyParams()
            params.mass = 50
            params.restitution = 0.5
            params.friction = 0.2
            params.linearDamping = 0.1
            params.angularDamping = 0.1
    
            local rigidBody = node:addComponent("RigidBody", params)
            rigidBody:setCollider(sl.BoxCollider.create(vec3(1, 1, 1)))
        end
    })
end
