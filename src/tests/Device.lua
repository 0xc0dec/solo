--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local device = solo.device

assert(device:getWindowTitle())
device:setWindowTitle("werwer")

assert(device:getCanvasSize())

device:saveScreenshot("abc")

device:setCursorCaptured()

assert(device:getLifetime() ~= nil)
assert(device:getTimeDelta() ~= nil)

assert(device:isWindowCloseRequested() ~= nil)
assert(device:isQuitRequested() ~= nil)

assert(device:isKeyPressed(solo.KeyCode.Digit0, true) ~= nil)
assert(device:isKeyReleased(solo.KeyCode.Digit0) ~= nil)

assert(device:getMouseMotion())
assert(device:isMouseButtonDown(solo.MouseButton.Left) ~= nil)
assert(device:isMouseButtonReleased(solo.MouseButton.Left) ~= nil)

device:update(function() end)

assert(device:getSetup())

assert(device:getFileSystem())
assert(device:getRenderer())
assert(device:getPhysics())
assert(device:getLogger())
