local demos = {
	{ "generic demo with several objects", "../tests/scripts/demo/demo1.lua" }
}

function runDemo(path)
	local args = solo.DeviceCreationArgs(solo.DeviceMode.OpenGL, 1366, 700, false, "Solo Tester")
	args.logFilePath = "demo.log"
	local device = solo.Device.create(args)
	local demo = dofile(path)
	demo:init(device)
	-- local _, err = pcall(function()
	-- 	demo:init(device)
	-- end)
	-- if err then
	-- 	device:getLogger():logCritical(err)
	-- end
	--
	device:run()

	demo = nil
	device = nil

	collectgarbage()
end

function prompt()
	io.write("Available demos:\n")
	for i, info in ipairs(demos) do
		print(i .. ": " .. info[1])
	end

	io.write("Which demo to run?: ")
	local number = tonumber(io.read())
	local demoInfo = demos[number]
	if demoInfo then
		runDemo(demoInfo[2])
		collectgarbage()
		return true
	else
		io.write("No such demo")
		return false
	end
end

while prompt() do
end
