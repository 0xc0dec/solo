local args = solo.DeviceCreationArgs(solo.DeviceMode.Stub, 1, 1)
args.logFilePath = "lua-tests.log"
device = solo.Device.create(args)
logger = device:getLogger()

logger:logInfo("Running Lua tests...")

function runTest(test, name)
	local msg = "Running " .. name .. " tests... "
	local _, err = select(1, pcall(test))
	if err then
		msg = msg .. "\n" .. err
		logger:logError(msg)
	else
		msg = msg .. "done"
		logger:logInfo(msg)
	end
end

dofile("../tests/scripts/tests.bit-flags.lua")
dofile("../tests/scripts/tests.bounding-box.lua")
dofile("../tests/scripts/tests.bounding-sphere.lua")
dofile("../tests/scripts/tests.camera.lua")
dofile("../tests/scripts/tests.component.lua")
dofile("../tests/scripts/tests.cube-texture.lua")
dofile("../tests/scripts/tests.device.lua")
dofile("../tests/scripts/tests.enums.lua")
dofile("../tests/scripts/tests.file-system.lua")
dofile("../tests/scripts/tests.frustum.lua")
dofile("../tests/scripts/tests.logger.lua")
dofile("../tests/scripts/tests.material.lua")
dofile("../tests/scripts/tests.matrix.lua")
dofile("../tests/scripts/tests.mesh-renderer.lua")
dofile("../tests/scripts/tests.mesh.lua")
dofile("../tests/scripts/tests.node.lua")
dofile("../tests/scripts/tests.plane.lua")
dofile("../tests/scripts/tests.quaternion.lua")
dofile("../tests/scripts/tests.frame-buffer.lua")
dofile("../tests/scripts/tests.resource-manager.lua")
dofile("../tests/scripts/tests.scene.lua")
dofile("../tests/scripts/tests.skybox-renderer.lua")
dofile("../tests/scripts/tests.spectator.lua")
dofile("../tests/scripts/tests.surface-renderer.lua")
dofile("../tests/scripts/tests.texture2d.lua")
dofile("../tests/scripts/tests.transform.lua")
dofile("../tests/scripts/tests.vector2.lua")
dofile("../tests/scripts/tests.vector3.lua")
dofile("../tests/scripts/tests.vector4.lua")
dofile("../tests/scripts/tests.vertex-format.lua")

logger:logInfo("Finished tests")
