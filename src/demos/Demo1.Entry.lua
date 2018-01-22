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
deviceSetup.windowTitle = "Demo 1"
deviceSetup.logFilePath = "Demo1.log"

runScript = "../../src/demos/common/Demo.lua"