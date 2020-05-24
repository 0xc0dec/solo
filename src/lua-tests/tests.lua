sl.includeDir("../../src/lua-tests")

require "common"

function runTest(moduleName)
    print('Running test ' .. moduleName)
    require(moduleName)
end

function run()
    runTest('vector2')
end

callSafe(run)