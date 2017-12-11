--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(dev, assetCache)
    local physics = dev:getPhysics()

    local effect = assetCache.getEffect("Color")
    local highlightMaterial = sl.Material.create(dev, effect)
    highlightMaterial:setFaceCull(sl.FaceCull.All)
    highlightMaterial:setPolygonMode(sl.PolygonMode.Wireframe)
    highlightMaterial:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    highlightMaterial:setVector4Parameter("color", vec4(1, 1, 0, 1))

    return {
        typeId = sl.getCmpId("Highlighter"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
            self.camera = self.node:findComponent("Camera")
        end,

        update = function(self)
            if self.prevHighlighted then
                self.prevHighlighted:setMaterial(0, self.prevHighlightedMaterial)
                self.prevHighlighted = nil
                self.prevHighlightedMaterial = nil
            end

            local mousePos = dev:getMousePosition()
            local ray = self.camera:canvasPointToWorldRay(mousePos)
            local from = ray:getOrigin()
            local to = from + ray:getDirection() * 100
            local hitResult = physics:rayTestFirst(from, to)

            if hitResult.body then
                local node = hitResult.body:getNode()
                local spawnedObject = node:findScriptComponent(sl.getCmpId("SpawnedObject"))
                if spawnedObject then
                    local renderer = node:findComponent("MeshRenderer")
                    local prevMaterial = renderer:getMaterial(0)
                    renderer:setMaterial(0, highlightMaterial)
                    self.prevHighlighted = renderer
                    self.prevHighlightedMaterial = prevMaterial
                end
            end
        end
    }
end