--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require 'common'

local tags = require 'tags'

return function(assetCache)
    local canvasSize = sl.device:canvasSize()

    function createTargets()
        local tex = sl.Texture2D.empty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGB8)
        tex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.None)
        tex:setWrap(sl.TextureWrap.ClampToEdge)
        
        local frameBuffer = sl.FrameBuffer.fromAttachments(sl.device, { tex })

        return tex, frameBuffer
    end

    local targetTex1, target1 = createTargets()    
    local targetTex2, target2 = createTargets()
    local targetTex3, target3 = createTargets()
    local targetTex4, target4 = createTargets()

    local grayscaleMat = sl.Material.fromEffect(sl.device, assetCache.effect('grayscale'))
    grayscaleMat:setDepthTest(false)
    grayscaleMat:setDepthWrite(false)
    grayscaleMat:setFaceCull(sl.FaceCull.None)
    grayscaleMat:setFloatParameter('variables:rightSeparator', 0.25)
    grayscaleMat:setTextureParameter('mainTex', targetTex1)

    local saturateMat = sl.Material.fromEffect(sl.device, assetCache.effect('saturate'))
    saturateMat:setDepthTest(false)
    saturateMat:setDepthWrite(false)
    saturateMat:setFaceCull(sl.FaceCull.None)
    saturateMat:setFloatParameter('variables:leftSeparator', 0.75)
    saturateMat:setFloatParameter('variables:rightSeparator', 1.0)
    saturateMat:setTextureParameter('mainTex', targetTex2)

    local vBlurMat = sl.Material.fromEffect(sl.device, assetCache.effect('blur_vertical'))
    vBlurMat:setDepthTest(false)
    vBlurMat:setDepthWrite(false)
    vBlurMat:setFaceCull(sl.FaceCull.None)
    vBlurMat:setFloatParameter('variables:leftSeparator', 0.25)
    vBlurMat:setFloatParameter('variables:rightSeparator', 0.75)
    vBlurMat:setTextureParameter('mainTex', targetTex3)

    local hBlurMat = sl.Material.fromEffect(sl.device, assetCache.effect('blur_horizontal'))
    hBlurMat:setDepthTest(false)
    hBlurMat:setDepthWrite(false)
    hBlurMat:setFaceCull(sl.FaceCull.None)
    hBlurMat:setFloatParameter('variables:leftSeparator', 0.25)
    hBlurMat:setFloatParameter('variables:rightSeparator', 0.75)
    hBlurMat:setTextureParameter('mainTex', targetTex4)

    return function(camera)
        local scene = camera:node():scene()
        local node = scene:createNode()
        local renderer = node:addComponent('MeshRenderer')
        renderer:setTag(tags.postProcessorStep)
        renderer:setMesh(assetCache.meshes.quad())

        camera:setRenderTarget(target1)

        return {
            render = function()
                renderer:setMaterial(0, grayscaleMat)
                camera:setRenderTarget(target2)
                camera:renderFrame(function() scene:render(tags.postProcessorStep) end)

                renderer:setMaterial(0, saturateMat)
                camera:setRenderTarget(target3)
                camera:renderFrame(function() scene:render(tags.postProcessorStep) end)

                renderer:setMaterial(0, vBlurMat)
                camera:setRenderTarget(target4)
                camera:renderFrame(function() scene:render(tags.postProcessorStep) end)

                renderer:setMaterial(0, hBlurMat)
                camera:setRenderTarget(nil)
                camera:renderFrame(function() scene:render(tags.postProcessorStep) end)

                camera:setRenderTarget(target1)
            end,

            cleanup = function()
                camera:setRenderTarget(nil)
                scene:removeNode(node)
            end
        }
    end
end
