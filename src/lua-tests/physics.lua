local scene = sl.Scene.empty(sl.device)
local ph = sl.device:physics()
assert(ph)

local boxCollider = sl.BoxCollider.create(sl.Vector3(1, 2, 3))
assert(boxCollider)

local layout = sl.VertexBufferLayout()
layout:addAttribute(sl.VertexAttributeUsage.Position)
layout:addAttribute(sl.VertexAttributeUsage.Normal)
local mesh = sl.Mesh.fromFile(sl.device, assetPath('meshes/box.dae'), layout)
local meshCollider = sl.StaticMeshCollider.fromMesh(mesh)
assert(meshCollider)

local params = sl.RigidBodyParams()
params.mass = 1
params.friction = 2
params.restitution = 3
params.linearDamping = 4
params.angularDamping = 5
params.kinematic = false
params.linearFactor = sl.Vector3(1, 2, 3)
params.angularFactor = sl.Vector3(2, 3, 4)

local node = scene:createNode()

local body = node:addComponent("RigidBody", params)
assert(body)

body:setCollider(boxCollider)

assert(body:isKinematic() ~= nil)
body:setKinematic(true)

assert(body:isStatic() ~= nil)
body:setStatic(true)

ph:setGravity(sl.Vector3(0, -1, 0))

local hit = ph:rayTestFirst(sl.Vector3(10, 10, 10), sl.Vector3(0, 0, 0))
assert(hit.body == body)
assert(hit.point)
assert(hit.normal)

assert(ph:rayTestAll(sl.Vector3(1, 2, 3), sl.Vector3(0, 0, 0)))