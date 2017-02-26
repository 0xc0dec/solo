--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

function createPostProcessor(device, camera, tag)
	local canvasSize = device:getCanvasSize()

    local quadRenderer = camera:getNode():addComponent("MeshRenderer")
    quadRenderer:setTag(tag)
    quadRenderer:setMesh(solo.Mesh.createFromPrefab(device, solo.MeshPrefab.Quad))

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
            camera:renderFrame(function(ctx) quadRenderer:render(ctx) end)
        end
    }
end

function createPostProcessor1(device, camera, tag, shaders)
    local canvasSize = device:getCanvasSize()

    local fbTex1 = solo.RectTexture.create(device)
    fbTex1:setData(solo.TextureFormat.RGB, {}, canvasSize.x, canvasSize.y)
    fbTex1:setFiltering(solo.TextureFiltering.Nearest)
    fbTex1:setWrapping(solo.TextureWrapping.Clamp)
    local fb1 = solo.FrameBuffer.create(device)
    fb1:setAttachments({ fbTex1 })

    local fbTex2 = solo.RectTexture.create(device)
    fbTex2:setData(solo.TextureFormat.RGB, {}, canvasSize.x, canvasSize.y)
    fbTex2:setFiltering(solo.TextureFiltering.Nearest)
    fbTex2:setWrapping(solo.TextureWrapping.Clamp)
    local fb2 = solo.FrameBuffer.create(device)
    fb2:setAttachments({ fbTex2 })

    local grayscaleEffect = solo.Effect.create(device, shaders.vs.passThrough, shaders.fs.grayscale)
    local grayscaleMat = solo.Material.create(device, grayscaleEffect)
    grayscaleMat:setDepthTest(false)
    grayscaleMat:setDepthWrite(false)
    grayscaleMat:setFaceCull(solo.FaceCull.All)
    grayscaleMat:setFloatParameter("rightSeparator", 0.25)

    local saturateEffect = solo.Effect.create(device, shaders.vs.passThrough, shaders.fs.saturate)
    local saturateMat = solo.Material.create(device, saturateEffect)
    saturateMat:setDepthTest(false)
    saturateMat:setDepthWrite(false)
    saturateMat:setFaceCull(solo.FaceCull.All)
    saturateMat:setFloatParameter("leftSeparator", 0.75)
    saturateMat:setFloatParameter("rightSeparator", 1.0)

    local verticalBlurEffect = solo.Effect.create(device, shaders.vs.passThrough, shaders.fs.verticalBlur)
    local verticalBlurMat = solo.Material.create(device, verticalBlurEffect)
    verticalBlurMat:setDepthTest(false)
    verticalBlurMat:setDepthWrite(false)
    verticalBlurMat:setFaceCull(solo.FaceCull.All)
    verticalBlurMat:setFloatParameter("leftSeparator", 0.25)
    verticalBlurMat:setFloatParameter("rightSeparator", 0.75)

    local horizontalBlurEffect = solo.Effect.create(device, shaders.vs.passThrough, shaders.fs.horizontalBlur)
    local horizontalBlurMat = solo.Material.create(device, horizontalBlurEffect)
    horizontalBlurMat:setDepthTest(false)
    horizontalBlurMat:setDepthWrite(false)
    horizontalBlurMat:setFaceCull(solo.FaceCull.All)
    horizontalBlurMat:setFloatParameter("leftSeparator", 0.25)
    horizontalBlurMat:setFloatParameter("rightSeparator", 0.75)

    camera:setRenderTarget(fb1)

    local pp = createPostProcessor(device, camera, tag)

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

function createPostProcessor2(device, loader, camera, tag, shaders, getAssetPath)
    local stitchWidth = 30
    local canvasSize = device:getCanvasSize()

    local stitchTex = loader:loadRectTexture(getAssetPath("Stitches.png"))
    stitchTex:setFiltering(solo.TextureFiltering.Nearest)

    local stitchTexSize = stitchTex:getSize()

    local resX = math.floor(canvasSize.x / stitchWidth) * 2
    resX = (resX >= 2048) and 2048 or resX

    local resY = math.floor(canvasSize.y / stitchTexSize.y) * 2
    resY = (resY >= 2048) and 2048 or resY

    local offscreenRes = vec2(resX, resY)

    local stitchCount = vec2(offscreenRes.x * stitchWidth / (2 * stitchTexSize.x), offscreenRes.y / 2)

    local fbTex = solo.RectTexture.create(device)
    fbTex:setData(solo.TextureFormat.RGB, {}, offscreenRes.x, offscreenRes.y)
    fbTex:setFiltering(solo.TextureFiltering.Nearest)
    fbTex:setWrapping(solo.TextureWrapping.Clamp)
    local fb1 = solo.FrameBuffer.create(device)
    fb1:setAttachments({ fbTex })

    local effect = solo.Effect.create(device, shaders.vs.passThrough, shaders.fs.stitches)
    local material = solo.Material.create(device, effect)
    material:setTextureParameter("mainTex", fbTex)
    material:setTextureParameter("stitchTex", stitchTex)
    material:setVector2Parameter("stitchCount", stitchCount)
    material:setVector2Parameter("resolution", offscreenRes)

    camera:setViewport(vec4(0, 0, offscreenRes.x, offscreenRes.y))
    camera:setRenderTarget(fb1)

    local pp = createPostProcessor(device, camera, tag)

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