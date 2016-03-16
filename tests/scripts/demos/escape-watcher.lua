return function(device)
	return {
		typeId = 100,

		update = function()
			if device:isKeyPressed(solo.KeyCode.Escape, true) then
				device:requestShutdown()
			end
		end
	}
end
