return function(device, axis, speed, space)
	return {
		typeId = 300,

		init = function(self)
			self.transform = self.node:findComponent("Transform")
		end,

		update = function(self)
			local angle = device:getTimeDelta() * speed
			local rotationSpace = solo.TransformSpace.World
			if space == "local" then
				rotationSpace = solo.TransformSpace.Self
			end
			self.transform:rotateAxisAngle(axis, angle, rotationSpace)
		end
	}
end
