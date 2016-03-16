return {
	createEscapeWatcher = dofile("../tests/scripts/demos/escape-watcher.lua"),

	createLocalXRotator = function(device)
		return {
			typeId = 200,

			init = function(self)
				self.transform = self.node:findComponent("Transform")
			end,

			update = function(self)
				local angle = device:getTimeDelta() * 1.3
				self.transform:rotateAxisAngle(solo.Vector3.unitX(), angle, solo.TransformSpace.Self)
			end
		}
	end,

	createWorldYRotator = function(device)
		return {
			typeId = 300,

			init = function(self)
				self.transform = self.node:findComponent("Transform")
			end,

			update = function(self)
				local angle = device:getTimeDelta()
				self.transform:rotateAxisAngle(solo.Vector3.unitY(), angle, solo.TransformSpace.World)
			end
		}
	end,

	createTargeter = function(target)
		return {
			typeId = 400,

			init = function(self)
				self.transform = self.node:findComponent("Transform")
			end,

			update = function(self)
				self.transform:lookAt(target:getWorldPosition(), solo.Vector3.unitY())
			end
		}
	end,
	
	createTimeMaterialUpdater = function(device)
		return {
			typeId = 600,

			init = function(self)
				self.material = self.node:findComponent("MeshRenderer"):getMaterial(0)
				self.time = 0
			end,

			update = function(self)
				self.time = self.time + device:getTimeDelta()
				self.material:setFloatParameter("time", self.time)
			end
		}
	end
}
