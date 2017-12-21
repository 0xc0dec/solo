--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(mesh, assetCache)
    local physics = sl.device:getPhysics()

    local effect = assetCache.getEffect("Color")

    local material = sl.Material.create(sl.device, effect)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setVector4Parameter("variables.color", vec4(1, 1, 0, 1))

    local highlightMaterial = sl.Material.create(sl.device, effect)
    highlightMaterial:setFaceCull(sl.FaceCull.None)
    highlightMaterial:setPolygonMode(sl.PolygonMode.Wireframe)
    highlightMaterial:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    highlightMaterial:setVector4Parameter("variables.color", vec4(1, 1, 0, 1))

    return {
        typeId = sl.getCmpId("Spawner"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
            self.scene = self.node:getScene()
            self.camera = self.node:findComponent("Camera")
        end,

        update = function(self)
            if sl.device:isKeyPressed(sl.KeyCode.Space, true) then
                self:spawn()
            end

            self:unhighlightPrev()

            if not sl.device:isMouseButtonDown(sl.MouseButton.Right, false) then
                local target = self:findUnderCursor()
                if target then
                    if sl.device:isMouseButtonDown(sl.MouseButton.Left, true) then
                        self.scene:removeNode(target)
                    else
                        self:highlight(target)
                    end
                end
            end
        end,

        spawn = function(self)
            local initialPos = self.transform:getLocalPosition() + self.transform:getLocalForward() * 3
            local initialRotation = self.transform:getLocalRotation()

            local node = self.scene:createNode()
            
            node:addScriptComponent({ typeId = sl.getCmpId("SpawnedObject") })
            
            local renderer = node:addComponent("MeshRenderer")
            renderer:setMesh(mesh)
            renderer:setMaterial(0, material)
    
            local transform = node:findComponent("Transform")
            transform:setLocalScale(vec3(0.3, 0.3, 0.3))
            transform:setLocalPosition(initialPos)
            transform:setLocalRotation(initialRotation)
    
            local params = sl.RigidBodyConstructionParameters()
            params.mass = 50
            params.restitution = 0.5
            params.friction = 0.2
            params.linearDamping = 0.1
            params.angularDamping = 0.1
    
            local rigidBody = node:addComponent("RigidBody", params)
            rigidBody:setCollider(sl.BoxCollider.create(vec3(1, 1, 1)))
        end,

        unhighlightPrev = function(self)
            if self.prevHighlighted then
                self.prevHighlighted:setMaterial(0, self.prevHighlightedMaterial)
                self.prevHighlighted = nil
                self.prevHighlightedMaterial = nil
            end
        end,

        findUnderCursor = function(self)
            local mousePos = sl.device:getMousePosition()
            local ray = self.camera:canvasPointToWorldRay(mousePos)
            local from = ray:getOrigin()
            local to = from + ray:getDirection() * 100
            local hitResult = physics:rayTestFirst(from, to)

            if hitResult.body then
                local node = hitResult.body:getNode()
                local spawnedObject = node:findScriptComponent(sl.getCmpId("SpawnedObject"))
                if spawnedObject then
                    return node
                end
            end

            return nil
        end,

        highlight = function(self, target)
            local renderer = target:findComponent("MeshRenderer")
            local prevMaterial = renderer:getMaterial(0)
            renderer:setMaterial(0, highlightMaterial)
            self.prevHighlighted = renderer
            self.prevHighlightedMaterial = prevMaterial
        end
    }
end