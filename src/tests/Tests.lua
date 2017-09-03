--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local logger = sl.device:getLogger()

function runTest(fileName)
    local test = assert(loadfile("../../src/tests/" .. fileName))
    local _, err = select(1, pcall(test))
    if err then
        logger:logError(fileName .. ": failed\n" .. err)
    else
        logger:logInfo(fileName .. ": success")
    end
end

runTest("DegreeRadian.lua")
runTest("Enums.lua")
runTest("Vector2.lua")
runTest("Vector3.lua")
runTest("Vector4.lua")
runTest("Quaternion.lua")
runTest("Matrix.lua")
runTest("Ray.lua")
runTest("Device.lua")
runTest("DeviceSetup.lua")
runTest("Logger.lua")
runTest("Scene.lua")
runTest("Node.lua")
runTest("Component.lua")
runTest("Transform.lua")
runTest("Camera.lua")
runTest("FrameBuffer.lua")
runTest("Texture.lua")
runTest("MeshRenderer.lua")
runTest("Effect.lua")
runTest("Material.lua")
runTest("Physics.lua")
runTest("FileSystem.lua")
runTest("Renderer.lua")
runTest("Mesh.lua")
runTest("Spectator.lua")
runTest("SkyboxRenderer.lua")
runTest("Font.lua")
runTest("FontRenderer.lua")
runTest("Lib.lua")

logger:logInfo("Script tests finished")
