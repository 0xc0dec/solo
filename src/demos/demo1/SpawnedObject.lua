--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(assetCache, mesh, initialPos, initialRotation)
    return {
        typeId = sl.getCmpId("SpawnedObject"),

        init = function(self)
            self.active = false

            local effect = assetCache.getEffect("Color")
            local material = sl.Material.create(sl.device, effect)
            material:setFaceCull(sl.FaceCull.All)
            material:setPolygonMode(sl.PolygonMode.Wireframe)
            material:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
            material:setVector4Parameter("color", vec4(1, 1, 0, 1))

            local renderer = self.node:addComponent("MeshRenderer")
            renderer:setMesh(mesh)
            renderer:setMaterial(0, material)
            
            local t = self.node:findComponent("Transform")
            t:setLocalScale(vec3(0.3, 0.3, 0.3))
            t:setLocalPosition(initialPos)
            t:setLocalRotation(initialRotation)

            local params = sl.RigidBodyConstructionParameters()
            params.mass = 50
            params.restitution = 0.5
            params.friction = 0.2
            params.linearDamping = 0.1
            params.angularDamping = 0.1

            local rigidBody = self.node:addComponent("RigidBody", params)
            rigidBody:setCollider(sl.BoxCollider.create(vec3(1, 1, 1)))

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

            self.activeTimer = self.activeTimer + sl.device:getTimeDelta()
            if self.activeTimer >= 0.2 then
                self:setColor(vec4(1, 1, 0, 1))
                self.active = false
            end
        end
    }
end