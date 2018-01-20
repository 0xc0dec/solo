--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

deviceSetup = sl.DeviceSetup.new()
deviceSetup.mode = sl.DeviceMode.OpenGL
deviceSetup.canvasWidth = 1200
deviceSetup.canvasHeight = 700
deviceSetup.fullScreen = false
deviceSetup.vsync = true
deviceSetup.windowTitle = "Test"
deviceSetup.logFilePath = "Test.log"

runScript = "../../src/test2.lua"