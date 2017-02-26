--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createDynamicQuadUpdater = dofile("../../src/demos/demo1/DynamicQuadUpdater.lua")

return function(dev, scene, effects, loadTextureAsync, getAssetPath)
    loadTextureAsync(getAssetPath("Freeman.png"), function(tex)
        tex:setWrapping(solo.TextureWrapping.Clamp)

        local layout = solo.VertexBufferLayout()
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

        local mesh = solo.Mesh.create(dev)
        mesh:addDynamicVertexBuffer(layout, data, 4)
        mesh:addPart(indices, 6)
        mesh:setPrimitiveType(solo.PrimitiveType.Triangles)

        local material = solo.Material.create(dev, effects.simpleTexture)
        material:setFaceCull(solo.FaceCull.All)
        material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
        material:setTextureParameter("mainTex", tex)

        local node = scene:createNode()
        node:findComponent("Transform"):setLocalPosition(vec3(0, 0, -5))
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(mesh)
        renderer:setMaterial(0, material)

        node:addScriptComponent(createDynamicQuadUpdater(dev, data, mesh))
    end)
end