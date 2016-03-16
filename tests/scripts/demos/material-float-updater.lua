return function(device, paramName)
	return {
		typeId = 600,

		init = function(self)
			self.material = self.node:findComponent("MeshRenderer"):getMaterial(0)
			self.time = 0
		end,

		update = function(self)
			self.time = self.time + device:getTimeDelta()
			self.material:setFloatParameter(paramName, self.time)
		end
	}
end
