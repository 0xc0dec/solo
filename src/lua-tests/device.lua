local d = sl.device
assert(d)

assert(d:windowTitle())
d:setWindowTitle('Test title')

assert(d:canvasSize())
assert(d:dpiIndependentCanvasSize())
assert(d:isVsync() ~= nil)
assert(d:mode())

d:setCursorCaptured(false)

assert(d:lifetime())
assert(d:timeDelta())
assert(d:isWindowCloseRequested() ~= nil)
assert(d:isQuitRequested() ~= nil)
assert(d:isKeyPressed(sl.KeyCode.Digit0, false) ~= nil)
assert(d:isKeyReleased(sl.KeyCode.Digit0) ~= nil)
assert(d:mouseMotion())
assert(d:mousePosition())
assert(d:isMouseButtonDown(sl.MouseButton.Left, false) ~= nil)
assert(d:isMouseButtonReleased(sl.MouseButton.Left) ~= nil)

d:update(function() end)

assert(d:hasActiveBackgroundJobs() ~= nil)
assert(d:fileSystem())
assert(d:physics())
assert(d:renderer())
