--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

deviceSetup = sl.DeviceSetup()
deviceSetup.mode = sl.DeviceMode.Vulkan
deviceSetup.canvasWidth = 1200
deviceSetup.canvasHeight = 700
deviceSetup.fullScreen = false
deviceSetup.windowTitle = "Demo 2"
deviceSetup.redBits = 5
deviceSetup.greenBits = 5
deviceSetup.blueBits = 5
deviceSetup.depthBits = 24
deviceSetup.alphaBits = 0
deviceSetup.vsync = false
deviceSetup.logFilePath = "Demo2.log"

runScript = "../../src/demos/demo2/Demo2.lua"