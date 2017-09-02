--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

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

return function(scene, effects, loadTexture)
    local tex = loadTexture(getAssetPath("Freeman.png"))
    tex:setWrapping(sl.TextureWrapping.Clamp)

    local layout = sl.VertexBufferLayout()
    layout:addAttribute(3, 0)
    layout:addAttribute(2, 1)

    local data = {
        -1, -1, 0,    0, 0,
        -1,  1, 0,    0, 1,
        1,  1, 0,    1, 1,
        1, -1, 0,    1, 0
    }

    local indices = {
        0, 1, 2,
        0, 2, 3
    }

    local mesh = sl.Mesh.create(sl.device)
    mesh:addDynamicVertexBuffer(layout, data, 4)
    mesh:addPart(indices, 6)
    mesh:setPrimitiveType(sl.PrimitiveType.Triangles)

    local material = sl.Material.create(sl.device, effects.simpleTexture)
    material:setFaceCull(sl.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:setTextureParameter("mainTex", tex)

    local node = scene:createNode()
    node:findComponent("Transform"):setLocalPosition(vec3(0, 0, -5))
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(mesh)
    renderer:setMaterial(0, material)

    node:addScriptComponent(createUpdater(data, mesh))
end