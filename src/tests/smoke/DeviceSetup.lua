--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local s = sl.DeviceSetup()

assert(s.mode)
assert(s.canvasWidth)
assert(s.canvasHeight)
assert(s.windowTitle)
assert(s.redBits ~= nil)
assert(s.greenBits ~= nil)
assert(s.blueBits ~= nil)
assert(s.depthBits ~= nil)
assert(s.alphaBits ~= nil)
assert(s.vsync ~= nil)
assert(s.fullScreen ~= nil)