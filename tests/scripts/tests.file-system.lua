runTest(function()
	local fs = device:getFileSystem()

	fs:iterateLines("abc", function(line)
		return true
	end)

	fs:readLines("abc")
	fs:readText("abc")
	fs:writeLines("abc", {"line1", "line2"})
end, "FileSystem")
