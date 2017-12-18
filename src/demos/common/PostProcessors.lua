--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

function createPostProcessor(camera, tag)
	local canvasSize = sl.device:getCanvasSize()

    local quadRenderer = camera:getNode():addComponent("MeshRenderer")
    quadRenderer:setTag(tag)
    quadRenderer:setMesh(sl.Mesh.createFromPrefab(sl.device, sl.MeshPrefab.Quad))

    return {
        detach = function()
            camera:getNode():removeComponent("MeshRenderer")
            camera:setRenderTarget(nil)
            camera:setViewport(vec4(0, 0, canvasSize.x, canvasSize.y))
        end,

        renderStep = function(mat, inputTexture, target, viewport)
            mat:setTextureParameter("mainTex", inputTexture)
            quadRenderer:setMaterial(0, mat);
            camera:setViewport(viewport)
            camera:setRenderTarget(target)
            camera:renderFrame(function() quadRenderer:render() end)
        end
    }
end

function createPostProcessor1(camera, tag, assetCache)
    local canvasSize = sl.device:getCanvasSize()

    local fbTex1 = sl.Texture2d.createEmpty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGB)
    fbTex1:setFiltering(sl.TextureFiltering.Nearest)
    fbTex1:setWrapping(sl.TextureWrapping.Clamp)
    local fb1 = sl.FrameBuffer.create(sl.device)
    fb1:setAttachments({ fbTex1 })

    local fbTex2 = sl.Texture2d.createEmpty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGB)
    fbTex2:setFiltering(sl.TextureFiltering.Nearest)
    fbTex2:setWrapping(sl.TextureWrapping.Clamp)
    local fb2 = sl.FrameBuffer.create(sl.device)
    fb2:setAttachments({ fbTex2 })

    local grayscaleMat = sl.Material.create(sl.device, assetCache.getEffect("Grayscale"))
    grayscaleMat:setDepthTest(false)
    grayscaleMat:setDepthWrite(false)
    grayscaleMat:setFaceCull(sl.FaceCull.None)

    if sl.device:getMode() == sl.DeviceMode.Vulkan then
        grayscaleMat:setFloatParameter("variables.rightSeparator", 0.25)
    elseif sl.device:getMode() == sl.DeviceMode.OpenGL then
        grayscaleMat:setFloatParameter("rightSeparator", 0.25)
    end

    local saturateMat = sl.Material.create(sl.device, assetCache.getEffect("Saturate"))
    saturateMat:setDepthTest(false)
    saturateMat:setDepthWrite(false)
    saturateMat:setFaceCull(sl.FaceCull.None)

    if sl.device:getMode() == sl.DeviceMode.Vulkan then
        saturateMat:setFloatParameter("variables.leftSeparator", 0.75)
        saturateMat:setFloatParameter("variables.rightSeparator", 1.0)
    elseif sl.device:getMode() == sl.DeviceMode.OpenGL then
        saturateMat:setFloatParameter("leftSeparator", 0.75)
        saturateMat:setFloatParameter("rightSeparator", 1.0)
    end

    local verticalBlurMat = sl.Material.create(sl.device, assetCache.getEffect("VerticalBlur"))
    verticalBlurMat:setDepthTest(false)
    verticalBlurMat:setDepthWrite(false)
    verticalBlurMat:setFaceCull(sl.FaceCull.None)

    if sl.device:getMode() == sl.DeviceMode.Vulkan then
        verticalBlurMat:setFloatParameter("variables.leftSeparator", 0.25)
        verticalBlurMat:setFloatParameter("variables.rightSeparator", 0.75)
    elseif sl.device:getMode() == sl.DeviceMode.OpenGL then
        verticalBlurMat:setFloatParameter("leftSeparator", 0.25)
        verticalBlurMat:setFloatParameter("rightSeparator", 0.75)
    end

    local horizontalBlurMat = sl.Material.create(sl.device, assetCache.getEffect("HorizontalBlur"))
    horizontalBlurMat:setDepthTest(false)
    horizontalBlurMat:setDepthWrite(false)
    horizontalBlurMat:setFaceCull(sl.FaceCull.None)

    if sl.device:getMode() == sl.DeviceMode.Vulkan then
        horizontalBlurMat:setFloatParameter("variables.leftSeparator", 0.25)
        horizontalBlurMat:setFloatParameter("variables.rightSeparator", 0.75)
    elseif sl.device:getMode() == sl.DeviceMode.OpenGL then
        horizontalBlurMat:setFloatParameter("leftSeparator", 0.25)
        horizontalBlurMat:setFloatParameter("rightSeparator", 0.75)
    end

    camera:setRenderTarget(fb1)

    local pp = createPostProcessor(camera, tag)

    pp.apply = function(self)
        local viewport = vec4(0, 0, canvasSize.x, canvasSize.y)

        self.renderStep(grayscaleMat, fbTex1, fb2, viewport)
        self.renderStep(saturateMat, fbTex2, fb1, viewport)
        self.renderStep(verticalBlurMat , fbTex1, fb2, viewport)
        self.renderStep(horizontalBlurMat, fbTex2, nil, viewport)

        camera:setRenderTarget(fb1)
    end

    return pp
end

function createPostProcessor2(camera, tag, assetCache)
    local stitchWidth = 30
    local canvasSize = sl.device:getCanvasSize()

    local stitchTex = sl.Texture2d.loadFromFile(sl.device, getAssetPath("textures/Stitches.png"))
    stitchTex:setFiltering(sl.TextureFiltering.Nearest)

    local stitchTexSize = stitchTex:getDimensions()

    local resX = math.floor(canvasSize.x / stitchWidth) * 2
    resX = (resX >= 2048) and 2048 or resX

    local resY = math.floor(canvasSize.y / stitchTexSize.y) * 2
    resY = (resY >= 2048) and 2048 or resY

    local offscreenRes = vec2(resX, resY)

    local stitchCount = vec2(offscreenRes.x * stitchWidth / (2 * stitchTexSize.x), offscreenRes.y / 2)

    local fbTex = sl.Texture2d.createEmpty(sl.device, offscreenRes.x, offscreenRes.y, sl.TextureFormat.RGB)
    fbTex:setFiltering(sl.TextureFiltering.Nearest)
    fbTex:setWrapping(sl.TextureWrapping.Clamp)
    local fb1 = sl.FrameBuffer.create(sl.device)
    fb1:setAttachments({ fbTex })

    local material = sl.Material.create(sl.device, assetCache.getEffect("Stitches"))
    material:setTextureParameter("mainTex", fbTex)
    material:setTextureParameter("stitchTex", stitchTex)

    if sl.device:getMode() == sl.DeviceMode.Vulkan then
        material:setVector2Parameter("variables.stitchCount", stitchCount)
        material:setVector2Parameter("variables.resolution", offscreenRes)
    elseif sl.device:getMode() == sl.DeviceMode.OpenGL then
        material:setVector2Parameter("stitchCount", stitchCount)
        material:setVector2Parameter("resolution", offscreenRes)
    end

    camera:setViewport(vec4(0, 0, offscreenRes.x, offscreenRes.y))
    camera:setRenderTarget(fb1)

    local pp = createPostProcessor(camera, tag)

    pp.apply = function(self)
        self.renderStep(material, fbTex, nil, vec4(0, 0, canvasSize.x, canvasSize.y))
        camera:setViewport(vec4(0, 0, offscreenRes.x, offscreenRes.y))
        camera:setRenderTarget(fb1)
    end

    return pp
end

return {
    create1 = createPostProcessor1,
    create2 = createPostProcessor2
}