sl.includeDir("../../src/lua-tests")

require "common"

function runTest(moduleName)
    print('Running test ' .. moduleName)
    require(moduleName)
end

function run()
    runTest('vector2')
    runTest('vector3')
    runTest('vector4')
    runTest('quaternion')
    runTest('radians')
    runTest('degrees')
    runTest('matrix')
    runTest('ray')
    runTest('device')
    runTest('camera')
    runTest('enums')
    runTest('font')
    runTest('material')
    runTest('mesh')
    runTest('frame-buffer')
    runTest('scene')
    runTest('mesh-renderer')
    runTest('spectator')
    runTest('effect')
end

callSafe(run)