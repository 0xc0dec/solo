--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require 'common'

local tags = require 'tags'
local assetCache = require 'asset-cache'

return function(camera)
    local canvasSize = sl.device:canvasSize()
    local scene = camera:node():scene()

    local node = scene:createNode()
    local renderer = node:addComponent('MeshRenderer')
    renderer:setTag(tags.postProcessor)
    renderer:setMesh(assetCache.meshes.quad())
    renderer:setMaterial(0, material);

    function render()
        scene:render(tags.postProcessor)
    end

    function makeProcessor(start, steps)
        return {
            begin = function()
                camera:setViewport(start.viewport and start.viewport or vec4(0, 0, canvasSize.x, canvasSize.y))
                camera:setRenderTarget(start.target)
                -- scene rendering happens AFTER this point
            end,

            render = function()
                -- scene rendering happens BEFORE this point
                for idx, step in ipairs(steps) do
                    local viewport = step.viewport or vec4(0, 0, canvasSize.x, canvasSize.y)
                    renderer:setMaterial(0, step.material)
                    camera:setViewport(viewport)
                    camera:setRenderTarget(step.target)
                    camera:renderFrame(render)
                end
            end,

            cleanup = function()
                camera:setViewport(vec4(0, 0, canvasSize.x, canvasSize.y))
                camera:setRenderTarget(nil) -- TODO probably not needed
            end
        }
    end

    function make1()
        local createTarget = function()
            local tex = sl.Texture2D.empty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGB8)
            tex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.None)
            tex:setWrap(sl.TextureWrap.ClampToEdge)
            
            return tex, sl.FrameBuffer.fromAttachments(sl.device, { tex })
        end

        local targetTex1, target1 = createTarget()    
        local targetTex2, target2 = createTarget()
        local targetTex3, target3 = createTarget()
        local targetTex4, target4 = createTarget()

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

        local verticalBlurMat = sl.Material.fromEffect(sl.device, assetCache.effect('blur_vertical'))
        verticalBlurMat:setDepthTest(false)
        verticalBlurMat:setDepthWrite(false)
        verticalBlurMat:setFaceCull(sl.FaceCull.None)
        verticalBlurMat:setFloatParameter('variables:leftSeparator', 0.25)
        verticalBlurMat:setFloatParameter('variables:rightSeparator', 0.75)
        verticalBlurMat:setTextureParameter('mainTex', targetTex3)

        local horizontalBlurMat = sl.Material.fromEffect(sl.device, assetCache.effect('blur_horizontal'))
        horizontalBlurMat:setDepthTest(false)
        horizontalBlurMat:setDepthWrite(false)
        horizontalBlurMat:setFaceCull(sl.FaceCull.None)
        horizontalBlurMat:setFloatParameter('variables:leftSeparator', 0.25)
        horizontalBlurMat:setFloatParameter('variables:rightSeparator', 0.75)
        horizontalBlurMat:setTextureParameter('mainTex', targetTex4)

        return makeProcessor({
            target = target1
        }, {
            [1] = {
                material = grayscaleMat,
                target = target2
            },
            [2] = {
                material = saturateMat,
                target = target3
            },
            [3] = {
                material = verticalBlurMat,
                target = target4
            },
            [4] = {
                material = horizontalBlurMat,
                target = nil
            }
        })
    end

    function make2()
        local stitchWidth = 30
        local canvasSize = sl.device:canvasSize()

        local stitchTex = sl.Texture2D.fromFile(sl.device, assetPath('textures/stitches.png'))
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
        local target = sl.FrameBuffer.fromAttachments(sl.device, { fbTex })

        local material = sl.Material.fromEffect(sl.device, assetCache.effect('stitches'))
        material:setFaceCull(sl.FaceCull.None)
        material:setTextureParameter('mainTex', fbTex)
        material:setTextureParameter('stitchTex', stitchTex)
        material:setVector2Parameter('variables:stitchCount', stitchCount)
        material:setVector2Parameter('variables:resolution', offscreenRes)

        return makeProcessor({
            viewport = vec4(0, 0, offscreenRes.x, offscreenRes.y),
            target = target
        }, {
            [1] = {
                material = material,
                target = nil
            }
        })
    end

    return {
        [1] = make1(),
        [2] = make2()
    }
end