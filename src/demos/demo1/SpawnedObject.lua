--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(device, effect, mesh, initialPos, initialRotation)
    return {
        typeId = solo.getCmpId("SpawnedObject"),

        init = function(self)
            self.active = false

            local material = solo.Material.create(device, effect)
            material:setFaceCull(solo.FaceCull.All)
            material:setPolygonMode(solo.PolygonMode.Wireframe)
            material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
            material:setVector4Parameter("color", vec4(1, 1, 0, 1))

            local renderer = self.node:addComponent("MeshRenderer")
            renderer:setMesh(mesh)
            renderer:setMaterial(0, material)
            
            local t = self.node:findComponent("Transform")
            t:setLocalScale(vec3(0.3, 0.3, 0.3))
            t:setLocalPosition(initialPos)
            t:setLocalRotation(initialRotation)

            local params = solo.RigidBodyConstructionParameters()
            params.mass = 50
            params.restitution = 0.5
            params.friction = 0.2
            params.linearDamping = 0.1
            params.angularDamping = 0.1

            local rigidBody = self.node:addComponent("RigidBody", params)
            rigidBody:setCollider(solo.BoxCollider.create(vec3(1, 1, 1)))

            self.activeTimer = 0
            self.material = material
        end,

        setActive = function(self)
            self:setColor(vec4(1, 0, 0, 1))
            self.active = true
            self.activeTimer = 0
        end,

        setColor = function(self, color)
            self.material:setVector4Parameter("color", color)
        end,

        update = function(self)
            if not self.active then
                return
            end

            self.activeTimer = self.activeTimer + device:getTimeDelta()
            if self.activeTimer >= 0.2 then
                self:setColor(vec4(1, 1, 0, 1))
                self.active = false
            end
        end
    }
end