--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "Rotator"

return function(scene, assetCache)
    local effect = assetCache.getEffect("TextureWithLighting")
    local mat = sl.Material.create(sl.device, effect)
    mat:setFaceCull(sl.FaceCull.None)
    mat:bindParameter("matrices:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    mat:bindParameter("matrices:invTranspWorld", sl.BindParameterSemantics.InverseTransposedWorldMatrix)
    mat:setTextureParameter("mainTex", assetCache.textures.cobbleStone)

    local node = scene:createNode()
    node:addScriptComponent(createRotator("local", vec3(0, 1, 0), 1))

    local renderer = node:addComponent("MeshRenderer")
    renderer:setMaterial(0, mat)

    local transform = node:findComponent("Transform")
    transform:setLocalRotation(sl.Quaternion.createFromAxisAngle(vec3(0, 0, 1), sl.Radians.fromRawDegrees(180)))
    transform:setLocalPosition(vec3(-3, -1, -5))

    sl.Mesh.loadFromFileAsync(sl.device, getAssetPath("meshes/Teapot.obj")):done(
        function(mesh)
            renderer:setMesh(mesh)
        end)
end