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
end

callSafe(run)