--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(assetCache)
    local effect = assetCache.getEffect("color")

    local material = sl.Material.create(sl.device, effect)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
    material:setVector4Parameter("variables:color", vec4(1, 1, 0, 1))

    local highlightMaterial = sl.Material.create(sl.device, effect)
    highlightMaterial:setFaceCull(sl.FaceCull.None)
    highlightMaterial:setPolygonMode(sl.PolygonMode.Wireframe)
    highlightMaterial:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
    highlightMaterial:setVector4Parameter("variables:color", vec4(1, 1, 0, 1))

    return sl.createComponent("Spawner", {
        init = function(self)
            self.transform = self.node:findComponent("Transform")
            self.scene = self.node:scene()
            self.camera = self.node:findComponent("Camera")
            self.tracer = self.node:findScriptComponent(sl.getCmpId("Tracer"))
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
                        self.tracer.hitNode = nil -- Reset so that it's not picked anywhere else
                    else
                        self:highlight(target)
                    end
                end
            end
        end,

        spawn = function(self)
            local initialPos = self.transform:localPosition() + self.transform:localForward() * 3
            local initialRotation = self.transform:localRotation()

            local node = self.scene:createNode()
            
            node:addScriptComponent(sl.createComponent("SpawnedObject", {}))
            
            local renderer = node:addComponent("MeshRenderer")
            renderer:setMesh(assetCache.meshes.getBox())
            renderer:setMaterial(0, material)

            local size = 0.1 + math.random()
    
            local transform = node:findComponent("Transform")
            transform:setLocalScale(vec3(size, size, size))
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
            local hitNode = self.tracer.hitNode

            if hitNode then
                local spawnedObject = hitNode:findScriptComponent(sl.getCmpId("SpawnedObject"))
                if spawnedObject then
                    return hitNode
                end
            end

            return nil
        end,

        highlight = function(self, target)
            local renderer = target:findComponent("MeshRenderer")
            local prevMaterial = renderer:material(0)
            renderer:setMaterial(0, highlightMaterial)
            self.prevHighlighted = renderer
            self.prevHighlightedMaterial = prevMaterial
        end
    })
end
