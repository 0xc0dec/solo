--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

setup = sl.DeviceSetup()
setup.mode = sl.DeviceMode.Vulkan
setup.canvasWidth = 1600
setup.canvasHeight = 900
setup.fullScreen = false
setup.vsync = true
setup.windowTitle = "Demo 1 (Vulkan)"

entry = "../../src/demos/lua/demo1/demo.lua"