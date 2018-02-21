--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)
local mr = scene:createNode():addComponent("MeshRenderer")
local mesh = sl.Mesh.create(sl.device)
local eff = sl.Effect.createFromSource(sl.device, [[
    // VERTEX
    ...
    // FRAGMENT
]])
local mat = sl.Material.create(sl.device, eff)

mr:render()

mr:setMesh(mesh)
assert(mr:getMesh())
mr:setMesh(nil)

mr:setMaterial(0, mat)
assert(mr:getMaterial(0))
assert(mr:getMaterialCount())
mr:setMaterial(0, nil)
