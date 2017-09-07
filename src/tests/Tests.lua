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

runTest("smoke/DegreeRadian.lua")
runTest("smoke/Enums.lua")
runTest("smoke/Vector2.lua")
runTest("smoke/Vector3.lua")
runTest("smoke/Vector4.lua")
runTest("smoke/Quaternion.lua")
runTest("smoke/Matrix.lua")
runTest("smoke/Ray.lua")
runTest("smoke/Device.lua")
runTest("smoke/DeviceSetup.lua")
runTest("smoke/Logger.lua")
runTest("smoke/Scene.lua")
runTest("smoke/Node.lua")
runTest("smoke/Component.lua")
runTest("smoke/Transform.lua")
runTest("smoke/Camera.lua")
runTest("smoke/FrameBuffer.lua")
runTest("smoke/Texture.lua")
runTest("smoke/MeshRenderer.lua")
runTest("smoke/Effect.lua")
runTest("smoke/Material.lua")
runTest("smoke/Physics.lua")
runTest("smoke/FileSystem.lua")
runTest("smoke/Renderer.lua")
runTest("smoke/Mesh.lua")
runTest("smoke/Spectator.lua")
runTest("smoke/SkyboxRenderer.lua")
runTest("smoke/Font.lua")
runTest("smoke/FontRenderer.lua")
runTest("smoke/Lib.lua")

logger:logInfo("Script tests finished")
