--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createScreenshoter = dofile("../../src/demos/common/Screenshoter.lua")
local createSpawner = dofile("../../src/demos/demo1/Spawner.lua")
local createSpawnedObjectTargeter = dofile("../../src/demos/demo1/SpawnedObjectTargeter.lua")

return function(dev, scene, physics, meshes, effects)
    local node = scene:createNode()
        
    local t = node:findComponent("Transform")
    t:setLocalPosition(vec3(0, 5, 10))
    t:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))
    
    node:addScriptComponent(createScreenshoter(dev, "Demo1.bmp"))
    node:addComponent("Spectator")
    node:addScriptComponent(createSpawnedObjectTargeter(physics))
    node:addScriptComponent(createSpawner(dev, meshes.cube, effects.color))

    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(0.0, 0.6, 0.6, 1.0))
    cam:setNear(0.05)

    return cam
end