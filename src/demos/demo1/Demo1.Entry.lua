--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

deviceSetup = sl.DeviceSetup()
deviceSetup.mode = sl.DeviceMode.OpenGL
deviceSetup.canvasWidth = 1200
deviceSetup.canvasHeight = 700
deviceSetup.fullScreen = false
deviceSetup.vsync = false
deviceSetup.windowTitle = "Demo 1"
deviceSetup.logFilePath = "Demo1.log"

runScript = "../../src/demos/demo1/Demo1.lua"