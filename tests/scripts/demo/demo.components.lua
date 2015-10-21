function createEscapeWatcher()
	return
	{
		typeId = "EscapeWatcher",

		update = function()
			if device:isKeyPressed(solo.KeyCode_Escape, true) then
				device:requestShutdown()
			end
		end
	}
end


function createSpectator()
	return
	{
		typeId = "Spectator",

		init = function(self)
			self.transform = self.node:findComponent("Transform")
		end,

		update = function(self)
			local mouseMotion = device:getMouseMotion();
			local dt = device:getTimeDelta();

			if device:isMouseButtonDown(solo.MouseButton_Right, true) then
				device:setCursorCaptured(true)
			end
			if device:isMouseButtonReleased(solo.MouseButton_Right) then
				device:setCursorCaptured(false)
			end

			if device:isMouseButtonDown(solo.MouseButton_Right, false) then
				if mouseMotion.x ~= 0 then
					self.transform:rotateAxisAngle(solo.Vector3.unitY(), 0.5 * dt * -mouseMotion.x, solo.TransformSpace_World)
				end
				if mouseMotion.y ~= 0 then
					self.transform:rotateAxisAngle(solo.Vector3.unitX(), 0.5 * dt * -mouseMotion.y, solo.TransformSpace_Self)
				end
			end

			local movement = solo.Vector3.zero()
			if device:isKeyPressed(solo.KeyCode_W, false) then movement:addVector3(self.transform:getLocalForward()) end
			if device:isKeyPressed(solo.KeyCode_S, false) then movement:addVector3(self.transform:getLocalBack()) end
			if device:isKeyPressed(solo.KeyCode_A, false) then movement:addVector3(self.transform:getLocalLeft()) end
			if device:isKeyPressed(solo.KeyCode_D, false) then movement:addVector3(self.transform:getLocalRight()) end
			if device:isKeyPressed(solo.KeyCode_Q, false) then movement:addVector3(self.transform:getLocalDown()) end
			if device:isKeyPressed(solo.KeyCode_E, false) then movement:addVector3(self.transform:getLocalUp()) end
			movement:normalize()
			movement:multScalar(dt * 10)
			self.transform:translateLocal(movement)
		end
	}
end


function createLocalXRotator()
	return
	{
		typeId = "RotatorAroundLocalXAxis",

		init = function(self)
			self.transform = self.node:findComponent("Transform")
		end,

		update = function(self)
			local angle = device:getTimeDelta() * 1.3
			self.transform:rotateAxisAngle(solo.Vector3.unitX(), angle, solo.TransformSpace_Self)
		end
	}
end


function createWorldYRotator()
	return
	{
		typeId = "RotatorAroundWorldYAxis",

		init = function(self)
			self.transform = self.node:findComponent("Transform")
		end,

		update = function(self)
			local angle = device:getTimeDelta()
			self.transform:rotateAxisAngle(solo.Vector3.unitY(), angle, solo.TransformSpace_World)
		end
	}
end


function createTargeter(target)
	return
	{
		typeId = "Targeter",

		init = function(self)
			self.transform = self.node:findComponent("Transform")
		end,

		update = function(self)
			self.transform:lookAt(target:getWorldPosition(), solo.Vector3.unitY())
		end
	}
end
