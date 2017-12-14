--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(scene, assetCache)
    local createUpdater = function(data, mesh)
        local time = 0

        return {
            typeId = sl.getCmpId("DynamicQuadUpdater"),

            update = function()
                time = time + 2 * sl.device:getTimeDelta()
                local offset = 0.5 * math.sin(time)
                data[3] = offset
                data[8] = -offset
                data[13] = offset
                data[18] = -offset
                mesh:updateDynamicVertexBuffer(0, 0, data, 4)
            end
        }
    end

    local layout = sl.VertexBufferLayout()
    layout:addNamedAttribute(3, "position");
    layout:addNamedAttribute(2, "texCoord0");

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

    local texHandle = sl.Texture2d.loadFromFileAsync(sl.device, getAssetPath("textures/Bricks.jpg"))
    texHandle:done(function(tex)
        local tex = sl.Texture2d.loadFromFile(sl.device, getAssetPath("textures/Bricks.jpg"))
        tex:generateMipmaps()
        tex:setFiltering(sl.TextureFiltering.LinearMipmapNearest)
        tex:setAnisotropyLevel(8)
        tex:setWrapping(sl.TextureWrapping.Clamp)

        local effect = assetCache.getEffect("Texture")
        local mat = sl.Material.create(sl.device, effect)
        mat:setFaceCull(sl.FaceCull.None)
        mat:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
        mat:setTextureParameter("mainTex", tex)    

        local node = scene:createNode()
        node:findComponent("Transform"):setLocalPosition(vec3(0, 0, -5))
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(mesh)
        renderer:setMaterial(0, mat)

        node:addScriptComponent(createUpdater(data, mesh))
    end)
end