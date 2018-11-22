--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

deviceSetup = sl.DeviceSetup()
deviceSetup.mode = sl.DeviceMode.OpenGL
deviceSetup.canvasWidth = 1600
deviceSetup.canvasHeight = 900
deviceSetup.fullScreen = false
deviceSetup.vsync = true
deviceSetup.windowTitle = "Demo 1 (OpenGL)"

runScript = "../../src/demos/lua/demo1/demo1.lua"