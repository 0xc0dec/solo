--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local s = sl.DeviceSetup.new()

assert(s.mode)
assert(s.canvasWidth)
assert(s.canvasHeight)
assert(s.windowTitle)
assert(s.vsync ~= nil)
assert(s.fullScreen ~= nil)