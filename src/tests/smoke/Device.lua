--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local device = sl.device

assert(device:getWindowTitle())
device:setWindowTitle("werwer")

assert(device:getCanvasSize())
assert(device:getDpiIndependentCanvasSize())
assert(device:isVsync() ~= nil)
assert(device:getMode() ~= nil)

device:saveScreenshot("abc")

device:setCursorCaptured()

assert(device:getLifetime() ~= nil)
assert(device:getTimeDelta() ~= nil)

assert(device:isWindowCloseRequested() ~= nil)
assert(device:isQuitRequested() ~= nil)

assert(device:isKeyPressed(sl.KeyCode.Digit0, true) ~= nil)
assert(device:isKeyReleased(sl.KeyCode.Digit0) ~= nil)

assert(device:hasActiveBackgroundJobs() ~= nil)

assert(device:getMouseMotion())
assert(device:getMousePosition())
assert(device:isMouseButtonDown(sl.MouseButton.Left) ~= nil)
assert(device:isMouseButtonReleased(sl.MouseButton.Left) ~= nil)

device:update(sl.Scene.create(device))

assert(device:getFileSystem())
assert(device:getPhysics())
assert(device:getLogger())
