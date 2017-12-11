--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(dev, mesh, assetCache)
    local effect = assetCache.getEffect("Color")
    local material = sl.Material.create(dev, effect)
    material:setFaceCull(sl.FaceCull.All)
    material:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setVector4Parameter("color", vec4(1, 1, 0, 1))

    function spawn(scene, pos, rotation)
        local node = scene:createNode()

        node:addScriptComponent({ typeId = sl.getCmpId("SpawnedObject") })
        
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(mesh)
        renderer:setMaterial(0, material)

        local transform = node:findComponent("Transform")
        transform:setLocalScale(vec3(0.3, 0.3, 0.3))
        transform:setLocalPosition(pos)
        transform:setLocalRotation(rotation)

        local params = sl.RigidBodyConstructionParameters()
        params.mass = 50
        params.restitution = 0.5
        params.friction = 0.2
        params.linearDamping = 0.1
        params.angularDamping = 0.1

        local rigidBody = node:addComponent("RigidBody", params)
        rigidBody:setCollider(sl.BoxCollider.create(vec3(1, 1, 1)))
    end

    return {
        typeId = sl.getCmpId("Spawner"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
            self.scene = self.node:getScene()
        end,

        update = function(self)
            if dev:isKeyPressed(sl.KeyCode.Space, true) then
                local initialPos = self.transform:getLocalPosition() + self.transform:getLocalForward() * 3
                local initialRotation = self.transform:getLocalRotation()
                spawn(self.scene, initialPos, initialRotation)
            end
        end
    }
end