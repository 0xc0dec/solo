--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(dev, scene, effects, tex, cubeMesh)
    local material = sl.Material.create(dev, effects.simpleTexture)
    material:setFaceCull(sl.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:setTextureParameter("mainTex", tex)

    local node = scene:createNode()
    
    local transform = node:findComponent("Transform")
    transform:setLocalScale(vec3(10, 0.1, 10))
    transform:setLocalPosition(vec3(0, -2, 0))

    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(cubeMesh)
    renderer:setMaterial(0, material)

    local params = sl.RigidBodyConstructionParameters()
    params.mass = 0
    params.friction = 0.5
    local body = node:addComponent("RigidBody", params)
    body:setCollider(sl.BoxCollider.create(vec3(1, 1, 1)))
end