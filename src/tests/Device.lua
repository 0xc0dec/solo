--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local device = sl.device

assert(device:getWindowTitle())
device:setWindowTitle("werwer")

assert(device:getCanvasSize())

device:saveScreenshot("abc")

device:setCursorCaptured()

assert(device:getLifetime() ~= nil)
assert(device:getTimeDelta() ~= nil)

assert(device:isWindowCloseRequested() ~= nil)
assert(device:isQuitRequested() ~= nil)

assert(device:isKeyPressed(sl.KeyCode.Digit0, true) ~= nil)
assert(device:isKeyReleased(sl.KeyCode.Digit0) ~= nil)

assert(device:getMouseMotion())
assert(device:isMouseButtonDown(sl.MouseButton.Left) ~= nil)
assert(device:isMouseButtonReleased(sl.MouseButton.Left) ~= nil)

device:update(function() end)

assert(device:getSetup())

assert(device:getFileSystem())
assert(device:getRenderer())
assert(device:getPhysics())
assert(device:getLogger())
