local scene = sl.Scene.empty(sl.device)
local node = scene:createNode()

local spectator = node:addComponent('Spectator')
assert(spectator)

spectator:setMovementSpeed(123)
assert(spectator:movementSpeed() == 123)

spectator:setMouseSensitivity(123)
assert(spectator:mouseSensitivity() == 123)
