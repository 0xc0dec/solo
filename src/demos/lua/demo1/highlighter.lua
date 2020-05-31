--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(assetCache, physics)
    local supportedEffect = assetCache.getEffect('shadowed')

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
                local renderer = hitNode:findComponent("MeshRenderer")
                local material = renderer:material(0)

                if material:effect() == supportedEffect then
                    self.lastNode = hitNode
                    self.lastRenderer = renderer
                    self.lastMaterial = material

                    local newMat = material:clone()
                    newMat:setFloatParameter('variables:highlighted', 1)
                    renderer:setMaterial(0, newMat)
                end
            end
        end,
    });
end