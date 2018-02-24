--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "rotator"

return function(scene, assetCache)
    local effect = assetCache.getEffect("texture-with-lighting")
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
    transform:setLocalPosition(vec3(-3, -1, -5))

    local layout = sl.VertexBufferLayout()
    layout:addSemanticAttribute(sl.VertexAttributeSemantics.Position)
    layout:addSemanticAttribute(sl.VertexAttributeSemantics.Normal)
    layout:addSemanticAttribute(sl.VertexAttributeSemantics.TexCoord)
    sl.Mesh.loadFromFileAsync(sl.device, getAssetPath("meshes/Teapot.obj"), layout):done(
        function(mesh)
            renderer:setMesh(mesh)
        end)
end