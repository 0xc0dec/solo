--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(assetCache, physics)
    local highlightEff = assetCache.getEffect("color")
    local highlightMat = sl.Material.fromEffect(sl.device, highlightEff)
    highlightMat:setFaceCull(sl.FaceCull.None)
    highlightMat:setPolygonMode(sl.PolygonMode.Wireframe)
    highlightMat:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
    highlightMat:setVector4Parameter("variables:color", vec4(1, 1, 0, 1))

    return sl.createComponent("Highlighter", {
        init = function(self)
            self.transform = self.node:findComponent("Transform") 
            self.scene = self.node:scene()
            self.camera = self.node:findComponent("Camera")
            self.tracer = self.node:findScriptComponent(sl.cmpNameToId("Tracer"))
        end,

        update = function(self)
            local hitNode = self.tracer.hitNode

            if self.lastNode then
                if not hitNode or hitNode ~= self.lastNode then
                    self.lastRenderer:setMaterial(0, self.lastMaterial)
                    self.lastRenderer = nil
                    self.lastNode = nil
                    self.lastMaterial = nil
                end
            end

            if hitNode and not self.lastNode then
                self.lastNode = hitNode
                self.lastRenderer = self.lastNode:findComponent("MeshRenderer")
                self.lastMaterial = self.lastRenderer:material(0)
                self.lastRenderer:setMaterial(0, highlightMat)
            end
        end,
    });
end