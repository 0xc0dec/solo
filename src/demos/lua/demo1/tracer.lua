--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(scene, assetCache)
    function createTargetMesh()
        local node = scene:createNode()

        local eff = assetCache.effect('color')

        local mat = sl.Material.fromEffect(sl.device, eff)
        mat:setFaceCull(sl.FaceCull.None)
        mat:bindParameter('matrices:wvp', sl.ParameterBinding.WorldViewProjectionMatrix)
        mat:setVector4Parameter('variables:color', vec4(1, 0, 0, 1))
    
        local renderer = node:addComponent('MeshRenderer')
        renderer:setMaterial(0, mat)
    
        local transform = node:findComponent('Transform')
        transform:setLocalScale(vec3(0.05, 0.05, 0.05))
    
        local layout = sl.VertexBufferLayout()
        layout:addAttribute(sl.VertexAttributeUsage.Position)
        layout:addAttribute(sl.VertexAttributeUsage.Normal)
        layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
        sl.Mesh.fromFileAsync(sl.device, assetPath('meshes/box.dae'), layout)
            :done(function(mesh) renderer:setMesh(mesh) end)

        return {
            node = node,
            transform = transform,
            renderer = renderer
        }
    end

    return sl.createComponent('Tracer', {
        init = function(self)
            self.transform = self.node:findComponent('Transform') 
            self.scene = self.node:scene()
            self.camera = self.node:findComponent('Camera')
            self.target = createTargetMesh()
        end,

        update = function(self)
            local canvasSize = sl.device:canvasSize()
            local mousePos = canvasSize / 2
            local ray = self.camera:windowPointToWorldRay(mousePos)
            local from = ray:origin()
            local to = from + ray:direction() * 100
            local hitResult = sl.device:physics():rayTestFirst(from, to)
            if hitResult.body then
                self.hitNode = hitResult.body:node()
                self.target.transform:setLocalPosition(hitResult.point)
                self.target.renderer:setEnabled(true)
            else
                self.hitNode = nil
                self.target.renderer:setEnabled(false)
            end
        end,
    });
end