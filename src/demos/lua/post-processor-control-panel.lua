--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local tags = require "tags"

return function(assetCache, mainCameraNode, postProcessor)
    local scene = mainCameraNode:scene()

    local effect = assetCache.getEffect("color")

    function createMaterial(color)
        local m = sl.Material.create(sl.device, effect)
        m:setFaceCull(sl.FaceCull.None)
        m:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
        m:setVector4Parameter("variables:color", color)
        return m
    end

    function createFont()
        local textureWidth = 1024
        local textureHeight = 1024
        local lineHeight = 60
        local firstChar = string.byte(" ")
        local charCount = string.byte("~") - string.byte(" ")
        local path = getAssetPath("fonts/aller.ttf")
        local font = sl.Font.loadFromFile(sl.device, path, lineHeight, textureWidth, textureHeight, firstChar, charCount, 2, 2)

        local mesh = sl.FontMesh.create(sl.device, font)

        local effect = assetCache.getEffect("font")
        local material = sl.Material.create(sl.device, effect)
        material:setFaceCull(sl.FaceCull.None)
        material:setBlend(true)
        material:setDepthTest(true)
        material:setDepthWrite(false)
        material:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
        material:setTextureParameter("mainTex", font:atlas())

        return {
            font = font,
            mesh = mesh,
            material = material
        }
    end

    local function createBtn(parentNode, localPos, localScale, material, ppMode, tipText)
        local node = scene:createNode()
        
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(assetCache.meshes.getBox())
        renderer:setMaterial(0, material)

        local transform = node:findComponent("Transform")
        if parentNode then
            transform:setParent(parentNode:findComponent("Transform"))
        end
        transform:setLocalPosition(localPos)
        transform:setLocalScale(localScale)

        local params = sl.RigidBodyConstructionParameters()
        params.mass = 1 -- non-zero otherwise being kinematic doesn't trigger transform sync for the body
        params.friction = 0.5
        local body = node:addComponent("RigidBody", params)
        body:setCollider(sl.BoxCollider.create(vec3(1, 1, 1)))
        body:setKinematic(true)

        node:addScriptComponent(sl.createComponent("PostProcessorControlPanelBtn", {
            tipText = tipText,

            update = function(self)
                if self.highlighted and sl.device:isMouseButtonDown(sl.MouseButton.Left, true) then
                    postProcessor:setMode(ppMode)
                end
            end
        }))
    end

    local baseMat = createMaterial(vec4(0, 1, 1, 1))
    local btnMat = createMaterial(vec4(0, 1, 0, 1))

    local root = scene:createNode()
    local rootTransform = root:findComponent("Transform")

    local btnScale = vec3(0.3, 0.1, 0.3)
    createBtn(root, vec3(-0.8, 0, 0), btnScale, btnMat, "1", "Enable PP mode 1")
    createBtn(root, vec3(0, 0, 0), btnScale, btnMat, "2", "Enable PP mode 2")
    createBtn(root, vec3(0.8, 0, 0), btnScale, btnMat, "none", "Disable PP")

    local textNode = scene:createNode()
    local textTransform = textNode:findComponent("Transform")
    textTransform:setParent(rootTransform)
    textTransform:setLocalScale(vec3(0.01, 0.01, 1))
    textTransform:setLocalPosition(vec3(-1.1, 0.4, -0.4))

    local cmp = sl.createComponent("PostProcessorControlPanel", {
        init = function(self)
            self.tracer = mainCameraNode:findScriptComponent(sl.getCmpId("Tracer"))
            self.font = createFont()
            self.fontRenderer = textNode:addComponent("MeshRenderer")
            self.fontRenderer:setMaterial(0, self.font.material)
            self.fontRenderer:setTag(tags.transparent)
        end,

        update = function(self)
            local btn
            if self.tracer.hitNode then
                btn = self.tracer.hitNode:findScriptComponent(sl.getCmpId("PostProcessorControlPanelBtn"));
            end

            if btn ~= self.lastHighlightedBtn and self.lastHighlightedBtn then
                self.lastHighlightedBtn.highlighted = false
                self.lastHighlightedBtn = nil
                self.fontRenderer:setMesh(nil)
            end

            if btn then
                btn.highlighted = true
                self.lastHighlightedBtn = btn
                self.font.mesh:setText(btn.tipText)
                self.fontRenderer:setMesh(self.font.mesh:mesh())
            end
        end
    })

    return {
        node = root,
        transform = rootTransform,
        cmp = cmp
    }
end