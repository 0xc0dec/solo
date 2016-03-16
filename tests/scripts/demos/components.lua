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

	createPostProcessor = function(device, sourceTexture, shaders)
		return {
			typeId = 500,

			init = function(self)
				self.time = 0
				self.separator = 0.5
				self.srcTexture = sourceTexture

				local resourceManager = device:getResourceManager()

				local effect1 = resourceManager:getOrCreateEffect(shaders.vertex.passThrough, shaders.fragment.postProcessHalfGrayscale)
				local effect2 = resourceManager:getOrCreateEffect(shaders.vertex.passThrough, shaders.fragment.postProcessHalfSaturate)

				self.material1 = resourceManager:getOrCreateMaterial(effect1, "demo/post-processor/material1")
				self.material1:setFloatParameter("leftSeparator", 0.1)
				self.material1:setFloatParameter("rightSeparator", 0.3)
				self.material2 = resourceManager:getOrCreateMaterial(effect2, "demo/post-processor/material2")
				self.material2:setFloatParameter("leftSeparator", 0.7)
				self.material2:setFloatParameter("rightSeparator", 0.9)

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
