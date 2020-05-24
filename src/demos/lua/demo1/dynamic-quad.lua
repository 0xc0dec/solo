--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(scene, assetCache)
    local createUpdater = function(data, mesh)
        local time = 0

        return sl.createComponent("DynamicQuadUpdater", {
            update = function()
                time = time + 2 * sl.device:timeDelta()
                data[3] = 0.5 * math.sin(time + 1)
                data[8] = 0.5 * math.sin(time + 2)
                data[13] = 0.5 * math.sin(time + 3)
                data[18] = 0.5 * math.sin(time + 4)
                mesh:updateVertexBuffer(0, 0, data, 4)
            end
        })
    end

    local data = {
        -1, -1, 0, 0, 0,
        -1,  1, 0, 0, 1,
         1,  1, 0, 1, 1,
         1, -1, 0, 1, 0
    }

    local indices = {
        0, 1, 2,
        0, 2, 3
    }

    local layout = sl.VertexBufferLayout()
    layout:addAttribute(sl.VertexAttributeUsage.Position)
    layout:addAttribute(sl.VertexAttributeUsage.TexCoord)

    local mesh = sl.Mesh.empty(sl.device)
    mesh:addDynamicVertexBuffer(layout, data, 4)
    mesh:addIndexBuffer(indices, 6)
    mesh:setPrimitiveType(sl.PrimitiveType.Triangles)

    local effect = assetCache.getEffect("texture")
    local material = sl.Material.fromEffect(sl.device, effect)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
    
    local texHandle = sl.Texture2D.fromFileAsync(sl.device, assetPath("textures/rock_color.jpg"), true)
    texHandle:done(function(tex)
        tex:setAnisotropyLevel(16)
        material:setTextureParameter("mainTex", tex)
    end)

    local node = scene:createNode()
    local transform = node:findComponent("Transform")
    
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(mesh)
    renderer:setMaterial(0, material)

    node:addScriptComponent(createUpdater(data, mesh))

    return {
        node = node,
        transform = transform,
        renderer = renderer
    }
end