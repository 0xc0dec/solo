--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

function createStep(parentCamera, material, tag, target)
    local scene = parentCamera:getNode():getScene()

    local cameraNode = scene:createNode()
    cameraNode:findComponent("Transform"):setParent(parentCamera:getNode():findComponent("Transform"))

    local camera = cameraNode:addComponent("Camera")
    camera:setZNear(0.05)
    camera:setRenderTarget(target)

    local quadRenderer = cameraNode:addComponent("MeshRenderer")
    quadRenderer:setTag(tag)
    quadRenderer:setMesh(sl.Mesh.createFromPrefab(sl.device, sl.MeshPrefab.Quad))
    quadRenderer:setMaterial(0, material); -- TODO setting nil as material here causes VK renderer to crash

    local render = function()
        quadRenderer:render()
    end

    return {
        apply = function()
            camera:renderFrame(render)
        end,

        destroy = function()
            scene:removeNode(cameraNode)
        end
    }
end

function createPostProcessor1(camera, tag, assetCache)
    local canvasSize = sl.device:getCanvasSize()

    local createTarget = function()
        local tex = sl.Texture2d.createEmpty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGB)
        tex:setFiltering(sl.TextureFiltering.Nearest)
        tex:setWrapping(sl.TextureWrapping.Clamp)
        
        local frameBuffer = sl.FrameBuffer.create(sl.device)
        frameBuffer:setAttachments({ tex })

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
    grayscaleMat:setFloatParameter("variables.rightSeparator", 0.25)
    grayscaleMat:setTextureParameter("mainTex", targetTex1)

    local saturateMat = sl.Material.create(sl.device, assetCache.getEffect("Saturate"))
    saturateMat:setDepthTest(false)
    saturateMat:setDepthWrite(false)
    saturateMat:setFaceCull(sl.FaceCull.None)
    saturateMat:setFloatParameter("variables.leftSeparator", 0.75)
    saturateMat:setFloatParameter("variables.rightSeparator", 1.0)
    saturateMat:setTextureParameter("mainTex", targetTex2)

    local verticalBlurMat = sl.Material.create(sl.device, assetCache.getEffect("VerticalBlur"))
    verticalBlurMat:setDepthTest(false)
    verticalBlurMat:setDepthWrite(false)
    verticalBlurMat:setFaceCull(sl.FaceCull.None)
    verticalBlurMat:setFloatParameter("variables.leftSeparator", 0.25)
    verticalBlurMat:setFloatParameter("variables.rightSeparator", 0.75)
    verticalBlurMat:setTextureParameter("mainTex", targetTex3)

    local horizontalBlurMat = sl.Material.create(sl.device, assetCache.getEffect("HorizontalBlur"))
    horizontalBlurMat:setDepthTest(false)
    horizontalBlurMat:setDepthWrite(false)
    horizontalBlurMat:setFaceCull(sl.FaceCull.None)
    horizontalBlurMat:setFloatParameter("variables.leftSeparator", 0.25)
    horizontalBlurMat:setFloatParameter("variables.rightSeparator", 0.75)
    horizontalBlurMat:setTextureParameter("mainTex", targetTex4)

    camera:setRenderTarget(target1)
    local step1 = createStep(camera, grayscaleMat, tag, target2)
    local step2 = createStep(camera, saturateMat, tag, target3)
    local step3 = createStep(camera, verticalBlurMat, tag, target4)
    local step4 = createStep(camera, horizontalBlurMat, tag, nil)

    return {
        apply = function(self)
            step1.apply()
            step2.apply()
            step3.apply()
            step4.apply()
        end,

        detach = function()
            step1.destroy()
            step2.destroy()
            step3.destroy()
            step4.destroy()
            camera:setRenderTarget(nil)
        end
    }
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
    local target1 = sl.FrameBuffer.create(sl.device)
    target1:setAttachments({ fbTex })

    local material = sl.Material.create(sl.device, assetCache.getEffect("Stitches"))
    material:setTextureParameter("mainTex", fbTex)
    material:setTextureParameter("stitchTex", stitchTex)
    material:setVector2Parameter("variables.stitchCount", stitchCount)
    material:setVector2Parameter("variables.resolution", offscreenRes)

    local step = createStep(camera, material, tag, nil)

    camera:setViewport(vec4(0, 0, offscreenRes.x, offscreenRes.y))
    camera:setRenderTarget(target1)

    return {
        apply = function(self)
            step.apply()
        end,

        detach = function()
            step.destroy()
            camera:setRenderTarget(nil)
            camera:setViewport(vec4(0, 0, canvasSize.x, canvasSize.y))
        end
    }
end

return {
    create1 = createPostProcessor1,
    create2 = createPostProcessor2
}