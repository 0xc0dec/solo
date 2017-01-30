local physics = solo.device:getPhysics()
local v1 = solo.Vector3(1, 2, 3)
local v2 = solo.Vector3(2, 3, 4)

physics:update()
physics:setGravity(v1)

local rcr = physics:castRay(v1, v2)
assert(rcr)
local b = rcr.body -- so it just compiles
assert(rcr.point)
assert(rcr.normal)

assert(physics:castRayAll(v1, v2))


local params = solo.RigidBodyConstructionParameters()

assert(params.mass ~= nil)
assert(params.friction ~= nil)
assert(params.restitution ~= nil)
assert(params.linearDamping ~= nil)
assert(params.angularDamping ~= nil)
assert(params.kinematic ~= nil)
assert(params.linearFactor ~= nil)
assert(params.angularFactor ~= nil)


local scene = solo.Scene.create(solo.device)
local collider = solo.BoxCollider.create(v1)
local body = scene:createNode():addComponent("RigidBody", solo.RigidBodyConstructionParameters())

body:setCollider(collider)
body:setCollider(nil)