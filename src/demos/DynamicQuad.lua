--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(scene, assetCache)
    local createUpdater = function(data, mesh)
        local time = 0

        return sl.createComponent("DynamicQuadUpdater", {
            update = function()
                time = time + 2 * sl.device:getTimeDelta()
                data[3] = 0.5 * math.sin(time + 1)
                data[8] = 0.5 * math.sin(time + 2)
                data[13] = 0.5 * math.sin(time + 3)
                data[18] = 0.5 * math.sin(time + 4)
                mesh:updateDynamicVertexBuffer(0, 0, data, 4)
            end
        })
    end

    local layout = sl.VertexBufferLayout()
    layout:addNamedAttribute(3, "position");
    layout:addNamedAttribute(2, "texCoord");

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

    local mesh = sl.Mesh.create(sl.device)
    mesh:addDynamicVertexBuffer(layout, data, 4)
    mesh:addPart(indices, 6)
    mesh:setPrimitiveType(sl.PrimitiveType.Triangles)

    local texHandle = sl.Texture2d.loadFromFileAsync(sl.device, getAssetPath("textures/Bricks.jpg"), true)
    texHandle:done(function(tex)
        tex:setAnisotropyLevel(16)

        -- TODO Move to a shared place - this material is used in many places
        local effect = assetCache.getEffect("Texture")
        local material = sl.Material.create(sl.device, effect)
        material:setFaceCull(sl.FaceCull.None)
        material:bindParameter("matrices:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
        material:setTextureParameter("mainTex", tex)

        local node = scene:createNode()
        node:findComponent("Transform"):setLocalPosition(vec3(3, 0, -5))
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(mesh)
        renderer:setMaterial(0, material)

        node:addScriptComponent(createUpdater(data, mesh))
    end)
end