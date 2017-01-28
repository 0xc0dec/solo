logger = solo.device:getLogger()

function runTest(name, test)
    local _, err = select(1, pcall(test))
    if err then
        logger:logError(name .. ": failed\n" .. err)
    else
        logger:logInfo(name .. ": success")
    end
end

runTest("Enums", assert(loadfile("../src/tests/enums.lua")))
runTest("Degree/Radian", assert(loadfile("../src/tests/degree-radian.lua")))
runTest("Vector2", assert(loadfile("../src/tests/vector2.lua")))
runTest("Vector3", assert(loadfile("../src/tests/vector3.lua")))
runTest("Vector4", assert(loadfile("../src/tests/vector4.lua")))
runTest("Quaternion", assert(loadfile("../src/tests/quaternion.lua")))
runTest("Matrix", assert(loadfile("../src/tests/matrix.lua")))
runTest("TransformMatrix", assert(loadfile("../src/tests/transform-matrix.lua")))
runTest("Ray", assert(loadfile("../src/tests/ray.lua")))
runTest("BoundingSphere", assert(loadfile("../src/tests/bounding-sphere.lua")))
runTest("BoundingBox", assert(loadfile("../src/tests/bounding-box.lua")))
runTest("Plane", assert(loadfile("../src/tests/plane.lua")))
runTest("Frustum", assert(loadfile("../src/tests/frustum.lua")))
runTest("Device", assert(loadfile("../src/tests/device.lua")))
runTest("Logger", assert(loadfile("../src/tests/logger.lua")))

logger:logInfo("Script tests finished")
