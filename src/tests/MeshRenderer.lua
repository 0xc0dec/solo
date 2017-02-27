--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = solo.Scene.create(solo.device)
local mr = scene:createNode():addComponent("MeshRenderer")
local mesh = solo.Mesh.create(solo.device)
local effect = solo.Effect.create(solo.device, {}, {})
local mat = solo.Material.create(solo.device, effect)

mr:render()

mr:setMesh(mesh)
assert(mr:getMesh())
mr:setMesh(nil)

mr:setMaterial(0, mat)
assert(mr:getMaterial(0))
assert(mr:getMaterialCount())
mr:setMaterial(0, nil)
