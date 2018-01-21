--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local physics = sl.device:getPhysics()
local v1 = sl.Vector3.new(1, 2, 3)
local v2 = sl.Vector3.new(2, 3, 4)

physics:setGravity(v1)

local rtr = physics:rayTestFirst(v1, v2)
assert(rtr)
local b = rtr.body -- so it just compiles
assert(rtr.point)
assert(rtr.normal)

assert(physics:rayTestAll(v1, v2))


local params = sl.RigidBodyConstructionParameters.new()

assert(params.mass ~= nil)
assert(params.friction ~= nil)
assert(params.restitution ~= nil)
assert(params.linearDamping ~= nil)
assert(params.angularDamping ~= nil)
assert(params.kinematic ~= nil)
assert(params.linearFactor ~= nil)
assert(params.angularFactor ~= nil)


local scene = sl.Scene.create(sl.device)
local collider = sl.BoxCollider.create(v1)
local body = scene:createNode():addRigidBody(sl.RigidBodyConstructionParameters.new())

body:setCollider(collider)
body:setCollider(nil)