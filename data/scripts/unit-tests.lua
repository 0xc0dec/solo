print("Running unit tests...")

function runTest(test, name)
	print("Running " .. name .. "...")
	local status, err = pcall(test)
	if err then
		print(err)
	end
	print("Finished " .. name .. "...")
end

dofile("../data/scripts/unit-tests.vector2.lua")
dofile("../data/scripts/unit-tests.vector3.lua")
dofile("../data/scripts/unit-tests.vector4.lua")
dofile("../data/scripts/unit-tests.plane.lua")
dofile("../data/scripts/unit-tests.quaternion.lua")
dofile("../data/scripts/unit-tests.bounding-sphere.lua")
dofile("../data/scripts/unit-tests.matrix.lua")
dofile("../data/scripts/unit-tests.frustum.lua")
dofile("../data/scripts/unit-tests.node.lua")
dofile("../data/scripts/unit-tests.device.lua")

print("Finished unit tests")
