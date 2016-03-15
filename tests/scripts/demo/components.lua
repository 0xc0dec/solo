function createEscapeWatcher()
	return
	{
		typeId = 100,

		update = function()
			if device:isKeyPressed(solo.KeyCode.Escape, true) then
				device:requestShutdown()
			end
		end
	}
end


function createLocalXRotator()
	return
	{
		typeId = 200,

		init = function(self)
			self.transform = self.node:findComponent("Transform")
		end,

		update = function(self)
			local angle = device:getTimeDelta() * 1.3
			self.transform:rotateAxisAngle(solo.Vector3.unitX(), angle, solo.TransformSpace.Self)
		end
	}
end


function createWorldYRotator()
	return
	{
		typeId = 300,

		init = function(self)
			self.transform = self.node:findComponent("Transform")
		end,

		update = function(self)
			local angle = device:getTimeDelta()
			self.transform:rotateAxisAngle(solo.Vector3.unitY(), angle, solo.TransformSpace.World)
		end
	}
end


function createTargeter(target)
	return
	{
		typeId = 400,

		init = function(self)
			self.transform = self.node:findComponent("Transform")
		end,

		update = function(self)
			self.transform:lookAt(target:getWorldPosition(), solo.Vector3.unitY())
		end
	}
end


function createPostProcessor(sourceTexture, shaders)
	return
	{
		typeId = 500,

		init = function(self)
			self.time = 0
			self.separator = 0.5
			self.srcTexture = sourceTexture

			local effect1 = resourceManager:getOrCreateEffect(shaders.vsPassThrough, shaders.fsPostProcessHalfGrayscale)
			local effect2 = resourceManager:getOrCreateEffect(shaders.vsPassThrough, shaders.fsPostProcessHalfSaturate)

			self.material1 = resourceManager:getOrCreateMaterial(effect1, "demo/post-processor/material1")
			self.material1:setFloatParameter("separator", 0.2)
			self.material2 = resourceManager:getOrCreateMaterial(effect2, "demo/post-processor/material2")
			self.material2:setFloatParameter("separator", 0.8)

			local canvasSize = device:getCanvasSize()
			local rtt = resourceManager:getOrCreateTexture2D("demo/post-processor/rtt")
			rtt:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
			rtt:setFiltering(solo.TextureFiltering.Nearest)
			rtt:setWrapping(solo.TextureWrapping.Clamp)
			self.finalRTT = rtt
			self.finalFb = resourceManager:getOrCreateFrameBuffer("demo/post-processor/fb")
			self.finalFb:setAttachments({ rtt })
		end,

		onAfterCameraRender = function(self)
			self.material1:setTextureParameter("mainTex", self.srcTexture)
			device:getGraphics():blit(self.material1, self.finalFb)

			self.material2:setTextureParameter("mainTex", self.finalRTT)
			device:getGraphics():blit(self.material2, nil)
		end
	}
end


function createTimeMaterialUpdater()
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
