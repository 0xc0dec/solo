return function(device, path)
	return {
		typeId = 5000,

		update = function()
			if device:isKeyPressed(solo.KeyCode.P, true) then
				device:saveScreenshot(path)
			end
		end
	}
end
