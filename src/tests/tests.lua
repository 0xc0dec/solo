--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local logger = solo.device:getLogger()

function runTest(fileName)
    local test = assert(loadfile("../../src/tests/" .. fileName))
    local _, err = select(1, pcall(test))
    if err then
        logger:logError(fileName .. ": failed\n" .. err)
    else
        logger:logInfo(fileName .. ": success")
    end
end

runTest("degree-radian.lua")
runTest("vector2.lua")
runTest("vector3.lua")
runTest("vector4.lua")
runTest("quaternion.lua")
runTest("matrix.lua")
runTest("transform-matrix.lua")
runTest("ray.lua")
runTest("bounding-sphere.lua")
runTest("bounding-box.lua")
runTest("plane.lua")
runTest("frustum.lua")
runTest("device.lua")
runTest("device-setup.lua")
runTest("logger.lua")
runTest("scene.lua")
runTest("node.lua")
runTest("component.lua")
runTest("transform.lua")
runTest("camera.lua")
runTest("frame-buffer.lua")
runTest("texture.lua")
runTest("mesh-renderer.lua")
runTest("effect.lua")
runTest("material.lua")
runTest("physics.lua")
runTest("file-system.lua")
runTest("asset-loader.lua")
runTest("renderer.lua")
runTest("mesh.lua")
runTest("spectator.lua")
runTest("skybox-renderer.lua")
runTest("font.lua")
runTest("font-renderer.lua")
runTest("lib.lua")

logger:logInfo("Script tests finished")
