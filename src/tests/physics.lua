local physics = solo.device:getPhysics()
local v1 = solo.Vector3(1, 2, 3)
local v2 = solo.Vector3(2, 3, 4)

physics:update()
physics:setGravity(v1)

local rcr = solo.RaycastResult()
local b = rcr.body -- so it just compiles
assert(rcr.point)
assert(rcr.normal)

local box = solo.BoxCollider.create(v1)

assert(physics:castRay(v1, v2))
assert(physics:castRayAll(v1, v2))

local scene = solo.Scene.create(solo.device)
local node = scene:createNode()

local params = solo.RigidBodyConstructionParameters()

local rb = node:addComponent("RigidBody", params)
assert(rb)
