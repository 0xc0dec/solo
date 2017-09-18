--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "Rotator"

return function(dev, scene, effects)
    sl.Texture2d.loadFromFileAsync(dev, getAssetPath("textures/Cobblestone.png")):done(
        function(tex)
            tex:generateMipmaps()
            tex:setFiltering(sl.TextureFiltering.LinearMipmapNearest)
            tex:setAnisotropyLevel(8)
            tex:setWrapping(sl.TextureWrapping.Clamp)

            local mat = sl.Material.create(dev, effects.basicLighting)
            mat:setFaceCull(sl.FaceCull.All)
            mat:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
            mat:bindParameter("invTransposedWorldMatrix", sl.BindParameterSemantics.InverseTransposedWorldMatrix)
            mat:setTextureParameter("mainTex", tex)

            sl.Mesh.loadFromFileAsync(dev, getAssetPath("meshes/Teapot.obj")):done(
                function(mesh)
                    local node = scene:createNode()
                    local renderer = node:addComponent("MeshRenderer")
                    renderer:setMesh(mesh)
                    renderer:setMaterial(0, mat)
                    node:findComponent("Transform"):setLocalPosition(vec3(0, 0, 0))
                    node:addScriptComponent(createRotator("local", vec3(1, 0, 0)))
                end)
        end)
end