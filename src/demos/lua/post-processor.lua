--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "common"

local tags = require "tags"

local function createStep(camera, material, target, assetCache)
    local scene = camera:node():scene()
    local node = scene:createNode()
    local quadRenderer = node:addComponent("MeshRenderer")
    quadRenderer:setTag(tags.postProcessorStep)
    quadRenderer:setMesh(assetCache.meshes.getQuad())
    quadRenderer:setMaterial(0, material); -- TODO setting nil as material here causes VK renderer to crash

    return {
        apply = function()
            camera:setZNear(0.05)
            camera:setRenderTarget(target)
            camera:renderFrame(function()
                quadRenderer:render()
            end)
        end,

        cleanup = function()
            scene:removeNode(node)
        end
    }
end

local function init1(camera, assetCache)
    local canvasSize = sl.device:canvasSize()

    local createTarget = function()
        local tex = sl.Texture2D.empty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGB8)
        tex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.None)
        tex:setWrap(sl.TextureWrap.ClampToEdge)
        
        local frameBuffer = sl.FrameBuffer.create(sl.device, { tex })

        return tex, frameBuffer
    end

    local targetTex1, target1 = createTarget()    
    local targetTex2, target2 = createTarget()
    local targetTex3, target3 = createTarget()
    local targetTex4, target4 = createTarget()

    local grayscaleMat = sl.Material.fromEffect(sl.device, assetCache.getEffect("grayscale"))
    grayscaleMat:setDepthTest(false)
    grayscaleMat:setDepthWrite(false)
    grayscaleMat:setFaceCull(sl.FaceCull.None)
    grayscaleMat:setFloatParameter("variables:rightSeparator", 0.25)
    grayscaleMat:setTextureParameter("mainTex", targetTex1)

    local saturateMat = sl.Material.fromEffect(sl.device, assetCache.getEffect("saturate"))
    saturateMat:setDepthTest(false)
    saturateMat:setDepthWrite(false)
    saturateMat:setFaceCull(sl.FaceCull.None)
    saturateMat:setFloatParameter("variables:leftSeparator", 0.75)
    saturateMat:setFloatParameter("variables:rightSeparator", 1.0)
    saturateMat:setTextureParameter("mainTex", targetTex2)

    local verticalBlurMat = sl.Material.fromEffect(sl.device, assetCache.getEffect("blur_vertical"))
    verticalBlurMat:setDepthTest(false)
    verticalBlurMat:setDepthWrite(false)
    verticalBlurMat:setFaceCull(sl.FaceCull.None)
    verticalBlurMat:setFloatParameter("variables:leftSeparator", 0.25)
    verticalBlurMat:setFloatParameter("variables:rightSeparator", 0.75)
    verticalBlurMat:setTextureParameter("mainTex", targetTex3)

    local horizontalBlurMat = sl.Material.fromEffect(sl.device, assetCache.getEffect("blur_horizontal"))
    horizontalBlurMat:setDepthTest(false)
    horizontalBlurMat:setDepthWrite(false)
    horizontalBlurMat:setFaceCull(sl.FaceCull.None)
    horizontalBlurMat:setFloatParameter("variables:leftSeparator", 0.25)
    horizontalBlurMat:setFloatParameter("variables:rightSeparator", 0.75)
    horizontalBlurMat:setTextureParameter("mainTex", targetTex4)

    local step1 = createStep(camera, grayscaleMat, target2, assetCache)
    local step2 = createStep(camera, saturateMat, target3, assetCache)
    local step3 = createStep(camera, verticalBlurMat, target4, assetCache)
    local step4 = createStep(camera, horizontalBlurMat, nil, assetCache)

    camera:setRenderTarget(target1)

    return {
        apply = function()
            step1:apply()
            step2:apply()
            step3:apply()
            step4:apply()
            camera:setRenderTarget(target1)
        end,

        cleanup = function()
            step1:cleanup()
            step2:cleanup()
            step3:cleanup()
            step4:cleanup()
            camera:setRenderTarget(nil)
        end
    }
end

local function init2(camera, assetCache)
    local stitchWidth = 30
    local canvasSize = sl.device:canvasSize()

    local stitchTex = sl.Texture2D.fromFile(sl.device, getAssetPath("textures/stitches.png"))
    stitchTex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.None)

    local stitchTexSize = stitchTex:dimensions()

    local resX = math.floor(canvasSize.x / stitchWidth) * 2
    resX = (resX >= 2048) and 2048 or resX

    local resY = math.floor(canvasSize.y / stitchTexSize.y) * 2
    resY = (resY >= 2048) and 2048 or resY

    local offscreenRes = vec2(resX, resY)

    local stitchCount = vec2(offscreenRes.x * stitchWidth / (2 * stitchTexSize.x), offscreenRes.y / 2)

    local fbTex = sl.Texture2D.empty(sl.device, offscreenRes.x, offscreenRes.y, sl.TextureFormat.RGB8)
    fbTex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.None)
    fbTex:setWrap(sl.TextureWrap.ClampToEdge)
    local target = sl.FrameBuffer.create(sl.device, { fbTex })

    local material = sl.Material.fromEffect(sl.device, assetCache.getEffect("stitches"))
    material:setFaceCull(sl.FaceCull.None)
    material:setTextureParameter("mainTex", fbTex)
    material:setTextureParameter("stitchTex", stitchTex)
    material:setVector2Parameter("variables:stitchCount", stitchCount)
    material:setVector2Parameter("variables:resolution", offscreenRes)

    local step = createStep(camera, material, nil, assetCache)

    function prepareCamera()
        camera:setViewport(vec4(0, 0, offscreenRes.x, offscreenRes.y))
        camera:setRenderTarget(target)
    end

    return {
        apply = function()
            camera:setViewport(vec4(0, 0, canvasSize.x, canvasSize.y))
            step:apply()
            prepareCamera()
        end,

        cleanup = function()
            step:cleanup()
            camera:setRenderTarget(nil)
            camera:setViewport(vec4(0, 0, canvasSize.x, canvasSize.y))
        end
    }
end

return function(assetCache, mainCamera)
    local pp = nil
    
    local function cleanupAndReset()
        if pp then
            pp:cleanup()
            pp = nil
        end
    end

    return {
        apply = function(self)
            if pp then
                pp:apply()
            end
        end,

        setMode = function(self, mode)
            cleanupAndReset()

            if mode == "1" then
                pp = init1(mainCamera, assetCache)
            elseif mode == "2" then
                pp = init2(mainCamera, assetCache)
            end
        end
    }
end
