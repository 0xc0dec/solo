print("Running unit tests...")

function runTest(test)
	local status, err = pcall(test)
	if err then
		print(err)
	end
end

dofile("../data/scripts/unit-tests.vector2.lua")
dofile("../data/scripts/unit-tests.vector3.lua")
dofile("../data/scripts/unit-tests.vector4.lua")
dofile("../data/scripts/unit-tests.node.lua")
dofile("../data/scripts/unit-tests.device.lua")

print("Finished unit tests")
