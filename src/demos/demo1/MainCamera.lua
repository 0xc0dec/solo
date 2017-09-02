--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createScreenshoter = require "Screenshoter"
local createSpawner = require "Spawner"
local createSpawnedObjectTargeter = require "SpawnedObjectTargeter"

return function(scene, physics, meshes, effects)
    local node = scene:createNode()
        
    local t = node:findComponent("Transform")
    t:setLocalPosition(vec3(0, 5, 10))
    t:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))
    
    node:addComponent("Spectator")
    node:addScriptComponent(createScreenshoter("Demo1.bmp"))
    node:addScriptComponent(createSpawnedObjectTargeter(physics))
    node:addScriptComponent(createSpawner(meshes.cube, effects.color))

    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(0.0, 0.6, 0.6, 1.0))
    cam:setNear(0.05)

    return cam
end