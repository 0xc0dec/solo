--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

setup = sl.DeviceSetup()
setup.mode = sl.DeviceMode.OpenGL
setup.canvasWidth = 1600
setup.canvasHeight = 900
setup.fullScreen = false
setup.vsync = true
setup.windowTitle = "Demo 2 (OpenGL)"

entry = "../../src/demos/lua/demo2/demo2.lua"