return function(device)
	return {
		typeId = 100,

		update = function()
			if device:isKeyPressed(solo.KeyCode.Escape, true) then
				device:stopRunning()
			end
		end
	}
end
