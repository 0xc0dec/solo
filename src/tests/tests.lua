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

runTest("enums.lua")
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
runTest("logger.lua")
runTest("scene.lua")
runTest("node-components.lua")

logger:logInfo("Script tests finished")
