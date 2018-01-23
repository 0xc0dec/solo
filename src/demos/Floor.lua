--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(scene, assetCache)
    local effect = assetCache.getEffect("Texture")
    local material = sl.Material.create(sl.device, effect)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setTextureParameter("mainTex", assetCache.textures.cobbleStone)

    local node = scene:createNode()
    
    local transform = node:findComponent("Transform")
    transform:setLocalScale(vec3(10, 0.1, 10))
    transform:setLocalPosition(vec3(0, -2, 0))

    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(assetCache.meshes.cube)
    renderer:setMaterial(0, material)

    local params = sl.RigidBodyConstructionParameters()
    params.mass = 0
    params.friction = 0.5
    local body = node:addComponent("RigidBody", params)
    body:setCollider(sl.BoxCollider.create(vec3(1, 1, 1)))
end