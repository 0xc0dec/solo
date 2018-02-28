--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(scene)
    local node = scene:createNode()
    
    node:addComponent("Spectator")

    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(0.0, 0.6, 0.6, 1.0))
    cam:setZNear(0.05)

    return cam, node
end