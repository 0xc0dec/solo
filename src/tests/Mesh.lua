--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local mesh1 = sl.Mesh.create(sl.device)
local mesh2 = sl.Mesh.createFromPrefab(sl.device, sl.MeshPrefab.Quad)

local data = sl.MeshData()
data.vertices = { sl.Vector3(1, 2, 3) }
data.uvs = { sl.Vector2(1, 2) }
data.normals = { sl.Vector3(1, 2, 3) }
data.indices = { { 1, 2, 3 }, { 1, 2, 3 } }
local mesh3 = sl.Mesh.createFromData(sl.device, data)

local layout = sl.VertexBufferLayout()
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
mesh1:setPrimitiveType(sl.PrimitiveType.Points)