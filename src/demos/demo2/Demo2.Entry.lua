--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

deviceSetup = sl.DeviceSetup()
deviceSetup.mode = sl.DeviceMode.Vulkan
deviceSetup.canvasWidth = 1000
deviceSetup.canvasHeight = 600
deviceSetup.logFilePath = "Demo2.log"

runScript = "../../src/demos/demo2/Demo2.lua"