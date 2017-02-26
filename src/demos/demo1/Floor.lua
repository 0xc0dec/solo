--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(dev, scene, effects, tex, cubeMesh)
    local material = solo.Material.create(dev, effects.simpleTexture)
    material:setFaceCull(solo.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:setTextureParameter("mainTex", tex)

    local node = scene:createNode()
    
    node:findComponent("Transform"):setLocalScale(vec3(10, 0.1, 10))
    node:findComponent("Transform"):setLocalPosition(vec3(0, -2, 0))

    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(cubeMesh)
    renderer:setMaterial(0, material)

    local params = solo.RigidBodyConstructionParameters()
    params.mass = 0
    params.friction = 0.5
    local body = node:addComponent("RigidBody", params)
    body:setCollider(solo.BoxCollider.create(vec3(1, 1, 1)))
end