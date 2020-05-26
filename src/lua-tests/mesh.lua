local layout = sl.VertexBufferLayout()
layout:addAttribute(sl.VertexAttributeUsage.Position)
layout:addAttribute(sl.VertexAttributeUsage.Normal)

local attr = layout:attribute(1)
assert(attr)
assert(attr.name == 'sl_Normal')
assert(attr.elementCount == 3)
assert(attr.size == 12)
assert(attr.offset == 12)
assert(attr.usage == sl.VertexAttributeUsage.Normal)

assert(layout:attributeCount() == 2)
assert(layout:attributeIndex(sl.VertexAttributeUsage.Normal) == 1)
assert(layout:size() == 24)
assert(layout:elementCount() == 6)

local m = sl.Mesh.empty(sl.device)
assert(m)

m = sl.Mesh.fromFile(sl.device, '../../assets/meshes/box.dae', layout)
assert(m)

-- TODO Properly check the result
assert(sl.Mesh.fromFileAsync(sl.device, '../../assets/meshes/box.dae', layout))

m = sl.Mesh.empty(sl.device)

m:addVertexBuffer(layout, {
    1, 2, 3,
    4, 5, 6
}, 1)

m:addDynamicVertexBuffer(layout, {
    1, 2, 3,
    4, 5, 6
}, 1)

m:updateVertexBuffer(1, 0, {0, 0, 0, 0, 0, 0}, 1)

m:removeVertexBuffer(1)

assert(m:vertexBufferCount() == 1)
assert(m:vertexBufferVertexCount(0) == 1)
assert(m:vertexBufferLayout(0))
assert(m:vertexBufferData(0))

m:addIndexBuffer({0, 0, 0}, 3)
assert(m:indexBufferCount() == 1)
assert(m:indexBufferElementCount(0) == 3)
assert(m:indexBufferElementSize(0) == 4)
assert(m:indexData(0))
m:removeIndexBuffer(0)

assert(m:primitiveType())
m:setPrimitiveType(sl.PrimitiveType.TriangleStrip)
