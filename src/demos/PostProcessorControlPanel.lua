--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(assetCache, mainCameraNode)
    local effect = assetCache.getEffect("Color")

    function createMaterial(color)
        local m = sl.Material.create(sl.device, effect)
        m:setFaceCull(sl.FaceCull.None)
        m:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
        m:setVector4Parameter("variables.color", color)
        return m
    end

    return sl.createComponent("PostProcessorControlPanel", {
        init = function(self)
            self.scene = self.node:getScene()
            self.postProcessor = mainCameraNode:findScriptComponent(sl.getCmpId("PostProcessor"))
            self.tracer = mainCameraNode:findScriptComponent(sl.getCmpId("Tracer"))

            local baseMat = createMaterial(vec4(0, 1, 1, 1))
            local btnMat = createMaterial(vec4(0, 1, 0, 1))

            local baseNode = self:_createBox(nil, vec3(0, -1.8, 6), vec3(1, 0.1, 0.5), baseMat, false)
            local btnScale = vec3(0.2, 1, 0.7)
            self.btn1Node = self:_createBox(baseNode, vec3(-0.6, 0.5, 0), btnScale, btnMat, true)
            self.btn2Node = self:_createBox(baseNode, vec3(0, 0.5, 0), btnScale, btnMat, true)
            self.btn3Node = self:_createBox(baseNode, vec3(0.6, 0.5, 0), btnScale, btnMat, true)
        end,

        update = function(self)
        end,

        _createBox = function(self, parentNode, localPos, localScale, material, withCollider)
            local node = self.scene:createNode()
            
            local renderer = node:addComponent("MeshRenderer")
            renderer:setMesh(assetCache.meshes.cube)
            renderer:setMaterial(0, material)
    
            local transform = node:findComponent("Transform")
            if parentNode then
                transform:setParent(parentNode:findComponent("Transform"))
            end
            transform:setLocalPosition(localPos)
            transform:setLocalScale(localScale)

            if withCollider then
                local params = sl.RigidBodyConstructionParameters()
                params.mass = 0
                params.friction = 0.5
                local body = node:addComponent("RigidBody", params)
                body:setCollider(sl.BoxCollider.create(vec3(1, 1, 1)))
            end

            return node
        end
    })
end