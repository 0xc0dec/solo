--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

deviceSetup = sl.DeviceSetup()
deviceSetup.mode = sl.DeviceMode.OpenGL
deviceSetup.canvasWidth = 1200
deviceSetup.canvasHeight = 700
deviceSetup.fullScreen = false
deviceSetup.windowTitle = "Lalala"
deviceSetup.redBits = 5
deviceSetup.greenBits = 5
deviceSetup.blueBits = 5
deviceSetup.depthBits = 24
deviceSetup.alphaBits = 0
deviceSetup.vsync = true
deviceSetup.logFilePath = "Demo1.log"

runScript = "../../src/demos/demo1/Demo1.lua"