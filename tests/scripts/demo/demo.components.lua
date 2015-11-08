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


function createPostProcessor(sourceTexture, material)
	return
	{
		typeId = "PostProcessor",

		init = function(self)
			self.graphics = device:getGraphics()
			self.srcTexture = sourceTexture

			local canvasSize = device:getCanvasSize()
			local renderTexture = resourceManager:getOrCreateTexture2D("post-process")
			renderTexture:setData(solo.ColorFormat_RGB, {}, canvasSize.x / 2, canvasSize.y / 2)
			renderTexture:setFiltering(solo.TextureFiltering_Nearest)
			renderTexture:setWrapping(solo.TextureWrapping_Clamp)
			self.finalRt = resourceManager:getOrCreateRenderTarget("post-process")
			self.finalRt:setColorAttachment(0, renderTexture)
			self.finalRtTexture = renderTexture
		end,

		onAfterCameraRender = function(self)
			self.graphics:renderImageToTarget(self.srcTexture, self.finalRt, material, "mainTex")
			self.graphics:renderImageToScreen(self.finalRtTexture, material, "mainTex")
		end
	}
end
