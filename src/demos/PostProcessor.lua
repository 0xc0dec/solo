--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

local tags = require "Tags"

local function createStep(parentCamera, material, target, cameraOrder, tag)
    local scene = parentCamera:getNode():getScene()

    local cameraNode = scene:createNode()
    cameraNode:findComponent("Transform"):setParent(parentCamera:getNode():findComponent("Transform"))

    local camera = cameraNode:addComponent("Camera")
    camera:setZNear(0.05)
    camera:setRenderTarget(target)
    camera:setOrder(cameraOrder)
    camera:setTagMask(tag)

    local quadRenderer = cameraNode:addComponent("MeshRenderer")
    quadRenderer:setTag(tag)
    quadRenderer:setMesh(sl.Mesh.createFromPrefab(sl.device, sl.MeshPrefab.Quad))
    quadRenderer:setMaterial(0, material); -- TODO setting nil as material here causes VK renderer to crash

    return function()
        scene:removeNode(cameraNode)
    end
end

local function init1(camera, assetCache)
    local canvasSize = sl.device:getCanvasSize()

    local createTarget = function()
        local tex = sl.Texture2D.createEmpty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGB)
        tex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.None)
        tex:setWrap(sl.TextureWrap.ClampToEdge)
        
        local frameBuffer = sl.FrameBuffer.create(sl.device, { tex })

        return tex, frameBuffer
    end

    local targetTex1, target1 = createTarget()    
    local targetTex2, target2 = createTarget()
    local targetTex3, target3 = createTarget()
    local targetTex4, target4 = createTarget()

    local grayscaleMat = sl.Material.create(sl.device, assetCache.getEffect("Grayscale"))
    grayscaleMat:setDepthTest(false)
    grayscaleMat:setDepthWrite(false)
    grayscaleMat:setFaceCull(sl.FaceCull.None)
    grayscaleMat:setFloatParameter("variables:rightSeparator", 0.25)
    grayscaleMat:setTextureParameter("mainTex", targetTex1)

    local saturateMat = sl.Material.create(sl.device, assetCache.getEffect("Saturate"))
    saturateMat:setDepthTest(false)
    saturateMat:setDepthWrite(false)
    saturateMat:setFaceCull(sl.FaceCull.None)
    saturateMat:setFloatParameter("variables:leftSeparator", 0.75)
    saturateMat:setFloatParameter("variables:rightSeparator", 1.0)
    saturateMat:setTextureParameter("mainTex", targetTex2)

    local verticalBlurMat = sl.Material.create(sl.device, assetCache.getEffect("VerticalBlur"))
    verticalBlurMat:setDepthTest(false)
    verticalBlurMat:setDepthWrite(false)
    verticalBlurMat:setFaceCull(sl.FaceCull.None)
    verticalBlurMat:setFloatParameter("variables:leftSeparator", 0.25)
    verticalBlurMat:setFloatParameter("variables:rightSeparator", 0.75)
    verticalBlurMat:setTextureParameter("mainTex", targetTex3)

    local horizontalBlurMat = sl.Material.create(sl.device, assetCache.getEffect("HorizontalBlur"))
    horizontalBlurMat:setDepthTest(false)
    horizontalBlurMat:setDepthWrite(false)
    horizontalBlurMat:setFaceCull(sl.FaceCull.None)
    horizontalBlurMat:setFloatParameter("variables:leftSeparator", 0.25)
    horizontalBlurMat:setFloatParameter("variables:rightSeparator", 0.75)
    horizontalBlurMat:setTextureParameter("mainTex", targetTex4)

    local cleanupStep1 = createStep(camera, grayscaleMat, target2, camera:getOrder() + 1, tags.postProcessorStep1)
    local cleanupStep2 = createStep(camera, saturateMat, target3, camera:getOrder() + 2, tags.postProcessorStep2)
    local cleanupStep3 = createStep(camera, verticalBlurMat, target4, camera:getOrder() + 3, tags.postProcessorStep3)
    local cleanupStep4 = createStep(camera, horizontalBlurMat, nil, camera:getOrder() + 4, tags.postProcessorStep4)

    camera:setRenderTarget(target1)

    return function()
        cleanupStep1()
        cleanupStep2()
        cleanupStep3()
        cleanupStep4()
        camera:setRenderTarget(nil)
    end
end

local function init2(camera, assetCache)
    local stitchWidth = 30
    local canvasSize = sl.device:getCanvasSize()

    local stitchTex = sl.Texture2D.loadFromFile(sl.device, getAssetPath("textures/Stitches.png"))
    stitchTex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.None)

    local stitchTexSize = stitchTex:getDimensions()

    local resX = math.floor(canvasSize.x / stitchWidth) * 2
    resX = (resX >= 2048) and 2048 or resX

    local resY = math.floor(canvasSize.y / stitchTexSize.y) * 2
    resY = (resY >= 2048) and 2048 or resY

    local offscreenRes = vec2(resX, resY)

    local stitchCount = vec2(offscreenRes.x * stitchWidth / (2 * stitchTexSize.x), offscreenRes.y / 2)

    local fbTex = sl.Texture2D.createEmpty(sl.device, offscreenRes.x, offscreenRes.y, sl.TextureFormat.RGB)
    fbTex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.None)
    fbTex:setWrap(sl.TextureWrap.ClampToEdge)
    local target = sl.FrameBuffer.create(sl.device, { fbTex })

    local material = sl.Material.create(sl.device, assetCache.getEffect("Stitches"))
    material:setFaceCull(sl.FaceCull.None)
    material:setTextureParameter("mainTex", fbTex)
    material:setTextureParameter("stitchTex", stitchTex)
    material:setVector2Parameter("variables:stitchCount", stitchCount)
    material:setVector2Parameter("variables:resolution", offscreenRes)

    local cleanupStep = createStep(camera, material, nil, camera:getOrder() + 1, tags.postProcessorStep1)

    camera:setViewport(vec4(0, 0, offscreenRes.x, offscreenRes.y))
    camera:setRenderTarget(target)

    return function()
        cleanupStep()
        camera:setRenderTarget(nil)
        camera:setViewport(vec4(0, 0, canvasSize.x, canvasSize.y))
    end
end

return function(assetCache)
    local cleanup = nil
    
    local function cleanupAndReset()
        if cleanup then
            cleanup()
            cleanup = nil
        end
    end

    return sl.createComponent("PostProcessor", {
        init = function(self)
            self.mainCamera = self.node:findComponent("Camera")
        end,

        setMode = function(self, mode)
            cleanupAndReset()

            if mode == "1" then
                cleanup = init1(self.mainCamera, assetCache)
            elseif mode == "2" then
                cleanup = init2(self.mainCamera, assetCache)
            end
        end
    })
end
