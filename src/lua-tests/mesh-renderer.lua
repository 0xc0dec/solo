local scene = sl.Scene.empty(sl.device)
local node = scene:createNode()

local renderer = node:addComponent('MeshRenderer')
assert(renderer)

local mesh = sl.Mesh.empty(sl.device)

renderer:setMesh(mesh)
assert(renderer:mesh() == mesh)

local effect = sl.Effect.fromDescriptionFile(sl.device, '../../assets/effects/test.lua')
local mat = sl.Material.fromEffect(sl.device, effect)

renderer:setMaterial(0, mat)
assert(renderer:material(0) == mat)

assert(renderer:materialCount() == 1)

renderer:setDefaultMaterial(mat)

renderer:render()
