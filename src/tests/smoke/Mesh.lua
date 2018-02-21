--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local mesh1 = sl.Mesh.create(sl.device)
local mesh2 = sl.Mesh.createFromPrefab(sl.device, sl.MeshPrefab.Quad)

local layout = sl.VertexBufferLayout()
layout:addSemanticAttribute(sl.VertexAttributeSemantics.Position)
assert(layout:getAttributeCount())
assert(layout:getSize())

local attr = layout:getAttribute(0)
assert(attr)
assert(attr.location)
assert(attr.size)
assert(attr.elementCount)
assert(attr.name)
assert(attr.offset)
assert(attr.semantics)

mesh1:addVertexBuffer(layout, { 1, 2, 3 }, 3)
mesh1:addDynamicVertexBuffer(layout, { 1, 2, 3 }, 3)
mesh1:updateDynamicVertexBuffer(0, 0, { 1, 2, 3 }, 3)
mesh1:removeVertexBuffer(0)

mesh1:addPart({ 1, 2, 3 }, 3)
mesh1:removePart(0)

assert(mesh1:getPartCount() ~= nil)

assert(mesh1:getPrimitiveType())
mesh1:setPrimitiveType(sl.PrimitiveType.Points)