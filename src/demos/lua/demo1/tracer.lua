--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(dev, scene, physics, assetCache)
    function createTargetMesh()
        local node = scene:createNode()

        local eff = assetCache.getEffect("color")

        local mat = sl.Material.fromEffect(dev, eff)
        mat:setFaceCull(sl.FaceCull.None)
        mat:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
        mat:setVector4Parameter("variables:color", vec4(1, 0, 0, 1))
    
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMaterial(0, mat)
    
        local transform = node:findComponent("Transform")
        transform:setLocalScale(vec3(0.05, 0.05, 0.05))
    
        local layout = sl.VertexBufferLayout()
        layout:addAttribute(sl.VertexAttributeUsage.Position)
        layout:addAttribute(sl.VertexAttributeUsage.Normal)
        layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
        sl.Mesh.fromFileAsync(dev, getAssetPath("meshes/box.dae"), layout)
            :done(function(mesh) renderer:setMesh(mesh) end)

        return {
            node = node,
            transform = transform,
            renderer = renderer,

            show = function()
                renderer:setMaterial(0, mat)
            end,

            hide = function()
                renderer:setMaterial(0, nil)
            end
        }
    end

    return sl.createComponent("Tracer", {
        init = function(self)
            self.transform = self.node:findComponent("Transform") 
            self.scene = self.node:scene()
            self.camera = self.node:findComponent("Camera")
            self.target = createTargetMesh()
        end,

        update = function(self)
            if sl.device:isMouseButtonDown(sl.MouseButton.Right, false) then
                self.hitNode = nil
                return
            end

            local mousePos = sl.device:mousePosition()
            local ray = self.camera:windowPointToWorldRay(mousePos)
            local from = ray:origin()
            local to = from + ray:direction() * 100
            local hitResult = physics:rayTestFirst(from, to)
            if hitResult.body then
                self.hitNode = hitResult.body:node()
                self.target.transform:setLocalPosition(hitResult.point)
                self.target.show()
            else
                self.hitNode = nil
                self.target.hide()
            end
        end,
    });
end