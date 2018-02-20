--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

deviceSetup = sl.DeviceSetup()
deviceSetup.mode = sl.DeviceMode.OpenGL
deviceSetup.canvasWidth = 1200
deviceSetup.canvasHeight = 700
deviceSetup.fullScreen = false
deviceSetup.vsync = true
deviceSetup.windowTitle = "Demo 1 (OpenGL)"
deviceSetup.logFilePath = "Demo1_gl.log"

runScript = "../../src/demos/Demo1.lua"