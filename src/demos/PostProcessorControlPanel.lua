--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(assetCache, mainCameraNode)
    local effect = assetCache.getEffect("Color")

    function createMaterial(color)
        local m = sl.Material.create(sl.device, effect)
        m:setFaceCull(sl.FaceCull.None)
        m:bindParameter("matrices:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
        m:setVector4Parameter("variables:color", color)
        return m
    end

    return sl.createComponent("PostProcessorControlPanel", {
        init = function(self)
            self.scene = self.node:getScene()
            self.postProcessor = mainCameraNode:findScriptComponent(sl.getCmpId("PostProcessor"))
            self.tracer = mainCameraNode:findScriptComponent(sl.getCmpId("Tracer"))

            local baseMat = createMaterial(vec4(0, 1, 1, 1))
            local btnMat = createMaterial(vec4(0, 1, 0, 1))

            local root = self.scene:createNode()
            root:findComponent("Transform"):setLocalPosition(vec3(0, -1.8, 6))

            local btnScale = vec3(0.3, 0.1, 0.3)
            self.btn1Node = self:_createBtn(root, vec3(-0.8, 0, 0), btnScale, btnMat, "1")
            self.btn2Node = self:_createBtn(root, vec3(0, 0, 0), btnScale, btnMat, "2")
            self.btn3Node = self:_createBtn(root, vec3(0.8, 0, 0), btnScale, btnMat, "none")
        end,

        update = function(self)
            if self.tracer.hitNode then
                local btn = self.tracer.hitNode:findScriptComponent(sl.getCmpId("PostProcessorControlPanelBtn"));
                if btn then
                    btn.highlighted = true
                    self.lastHighlightedBtn = btn
                elseif self.lastHighlightedBtn then
                    self.lastHighlightedBtn.highlighted = false
                    self.lastHighlightedBtn = nil
                end
            end
        end,

        _createBtn = function(self, parentNode, localPos, localScale, material, ppMode)
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

            local params = sl.RigidBodyConstructionParameters()
            params.mass = 0
            params.friction = 0.5
            local body = node:addComponent("RigidBody", params)
            body:setCollider(sl.BoxCollider.create(vec3(1, 1, 1)))

            node:addScriptComponent(sl.createComponent("PostProcessorControlPanelBtn", {
                update = function(self)
                    if self.highlighted and sl.device:isMouseButtonDown(sl.MouseButton.Left, true) then
                        local pp = mainCameraNode:findScriptComponent(sl.getCmpId("PostProcessor"))
                        pp:setMode(ppMode)
                    end
                end
            }))

            return node
        end
    })
end