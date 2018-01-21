--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "Rotator"

return function(scene, assetCache)
    local effect = assetCache.getEffect("TextureWithLighting")
    local mat = sl.Material.create(sl.device, effect)
    mat:setFaceCull(sl.FaceCull.None)
    mat:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    mat:bindParameter("matrices.invTranspWorld", sl.BindParameterSemantics.InverseTransposedWorldMatrix)
    mat:setTextureParameter("mainTex", assetCache.textures.cobbleStone)

    sl.Mesh.loadFromFileAsync(sl.device, getAssetPath("meshes/Teapot.obj")):done(
        function(mesh)
            local node = scene:createNode()
            local renderer = node:addMeshRenderer()
            renderer:setMesh(mesh)
            renderer:setMaterial(0, mat)
            node:findTransform():setLocalPosition(vec3(0, 0, 0))
            node:addScriptComponent(createRotator("local", vec3(1, 0, 0), 1))
        end)
end