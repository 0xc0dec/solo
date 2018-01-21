--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)
local mr = scene:createNode():addMeshRenderer()
local mesh = sl.Mesh.create(sl.device)
local mat = sl.Material.createFromPrefab(sl.device, sl.MaterialPrefab.Skybox)

mr:setMesh(mesh)
assert(mr:getMesh())
mr:setMesh(nil)

mr:setMaterial(0, mat)
assert(mr:getMaterial(0))
assert(mr:getMaterialCount())
mr:setMaterial(0, nil)
