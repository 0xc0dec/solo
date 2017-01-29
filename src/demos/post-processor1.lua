function createPostProcessor(device, camera, tag)
    local quadRenderer = camera:getNode():addComponent("MeshRenderer")
    quadRenderer:setTags(tag)
    quadRenderer:setMesh(solo.Mesh.create(device, solo.MeshPrefab.Quad))

    return {
        renderStep = function(mat, inputTexture, target, viewport)
            mat:setTextureParameter("mainTex", inputTexture)
            quadRenderer:setMaterial(0, mat);
            camera:setViewport(solo.Vector4(viewport.x, viewport.y, viewport.z, viewport.w))
            camera:setRenderTarget(target)
            camera:renderFrame(function(ctx)
                quadRenderer:render(ctx)
            end)
        end
    }
end

return function(device, camera, tag, shaders)
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
        local viewport = solo.Vector4(-1, -1, -1, -1)

        self.renderStep(grayscaleMat, fbTex1, fb2, viewport)
        self.renderStep(saturateMat, fbTex2, fb1, viewport)
        self.renderStep(verticalBlurMat , fbTex1, fb2, viewport)
        -- self.renderStep(horizontalBlurMat, fbTex2, nil, viewport)

        camera:setRenderTarget(fb1)
    end

    return pp
end