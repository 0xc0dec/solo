--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

deviceSetup = sl.DeviceSetup.new()
deviceSetup.mode = sl.DeviceMode.Null
deviceSetup.canvasWidth = 100
deviceSetup.canvasHeight = 100
deviceSetup.logFilePath = "Tests.log"

runScript = "../../src/tests/Tests.lua"