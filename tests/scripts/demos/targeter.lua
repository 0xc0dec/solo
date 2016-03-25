return function(targetPos)
	return {
		typeId = 400,

		init = function(self)
			self.transform = self.node:findComponent("Transform")
		end,

		update = function(self)
			self.transform:lookAt(targetPos, solo.Vector3.unitY())
		end
	}
end
