--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local mesh1 = solo.Mesh.create(solo.device)
local mesh2 = solo.Mesh.createFromPrefab(solo.device, solo.MeshPrefab.Quad)

local data = solo.MeshData()
data.vertices = { solo.Vector3(1, 2, 3) }
data.uvs = { solo.Vector2(1, 2) }
data.normals = { solo.Vector3(1, 2, 3) }
data.indices = { { 1, 2, 3 }, { 1, 2, 3 } }
local mesh3 = solo.Mesh.createFromData(solo.device, data)

local layout = solo.VertexBufferLayout()
layout:addAttribute(1, 0)
assert(layout:getAttributeCount())
assert(layout:getSize())

local attr = layout:getAttribute(0)
assert(attr)
assert(attr.location)
assert(attr.size)
assert(attr.elementCount)

mesh1:addVertexBuffer(layout, { 1, 2, 3 }, 3)
mesh1:addDynamicVertexBuffer(layout, { 1, 2, 3 }, 3)
mesh1:updateDynamicVertexBuffer(0, 0, { 1, 2, 3 }, 3)
mesh1:removeVertexBuffer(0)

mesh1:addPart({ 1, 2, 3 }, 3)
mesh1:removePart(0)

assert(mesh1:getPartCount() ~= nil)

assert(mesh1:getPrimitiveType())
mesh1:setPrimitiveType(solo.PrimitiveType.Points)