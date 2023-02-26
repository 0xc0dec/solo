--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require 'common'

return function(scene)
    return function(path, material)
        local node = scene:createNode()
        
        local body

        local renderer = node:addComponent('MeshRenderer')
        renderer:setDefaultMaterial(material)

        local layout = sl.VertexBufferLayout()
        layout:addAttribute(sl.VertexAttributeUsage.Position)
        layout:addAttribute(sl.VertexAttributeUsage.Normal)
        layout:addAttribute(sl.VertexAttributeUsage.Tangent)
        layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
        sl.Mesh.fromFileAsync(sl.device, assetPath(path), layout)
            :done(function(mesh)
                renderer:setMesh(mesh)
                body:setCollider(sl.StaticMeshCollider.fromMesh(mesh))
            end)

        local bodyParams = sl.RigidBodyParams()
        bodyParams.mass = 0
        bodyParams.friction = 0.5
        body = node:addComponent('RigidBody', bodyParams)
        body:setStatic(true)

        return {
            node = node,
            transform = node:findComponent('Transform'),
            renderer = renderer
        }
    end
end