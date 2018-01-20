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

function runTest(fileName)
    local test = assert(loadfile("../../src/tests/" .. fileName))
    local _, err = select(1, pcall(test))
    if err then
        print(fileName .. ": failed\n" .. err)
    else
        print(fileName .. ": success")
    end
end

runTest("smoke/Vector2.lua")

runScript = "../../src/test2.lua"