--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

deviceSetup = sl.DeviceSetup()
deviceSetup.mode = sl.DeviceMode.Vulkan
deviceSetup.canvasWidth = 1600
deviceSetup.canvasHeight = 900
deviceSetup.fullScreen = false
deviceSetup.vsync = true
deviceSetup.windowTitle = "Demo 2 (Vulkan)"

runScript = "../../src/demos/lua/demo2/demo2.lua"