print("Running unit tests...")

function runTest(test, name)
	io.write("Running " .. name .. " tests... ")
	local _, err = select(1, pcall(test))
	if err then
		print("\n" .. err)
	else
		print("done")
	end
end

engine = solo.Engine.create(solo.EngineCreationArgs(EngineMode_Stub, 1, 1))

dofile("../data/scripts/unit-tests.vector2.lua")
dofile("../data/scripts/unit-tests.vector3.lua")
dofile("../data/scripts/unit-tests.vector4.lua")
dofile("../data/scripts/unit-tests.plane.lua")
dofile("../data/scripts/unit-tests.quaternion.lua")
dofile("../data/scripts/unit-tests.bounding-box.lua")
dofile("../data/scripts/unit-tests.bounding-sphere.lua")
dofile("../data/scripts/unit-tests.matrix.lua")
dofile("../data/scripts/unit-tests.frustum.lua")
dofile("../data/scripts/unit-tests.node.lua")
dofile("../data/scripts/unit-tests.transform.lua")
dofile("../data/scripts/unit-tests.engine.lua")
dofile("../data/scripts/unit-tests.device.lua")

print("Finished unit tests\n")
