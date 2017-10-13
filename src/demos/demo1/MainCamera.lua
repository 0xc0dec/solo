--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createScreenshoter = require "Screenshoter"

return function(dev, scene)
    local node = scene:createNode()
        
    local t = node:findComponent("Transform")
    t:setLocalPosition(vec3(0, 5, 10))
    t:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))
    
    node:addComponent("Spectator")
    node:addScriptComponent(createScreenshoter("Demo1.bmp"))

    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(0.0, 0.6, 0.6, 1.0))
    cam:setNear(0.05)

    return cam, node
end