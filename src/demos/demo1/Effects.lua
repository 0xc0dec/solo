--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

return function()
    local shaderFiles = {
        vs = {
            basic = getAssetPath("shaders/gl/Basic.vert.glsl"),
            passThrough = getAssetPath("shaders/gl/PassThrough.vert.glsl"),
            basicLighting = getAssetPath("shaders/gl/BasicLighting.vert.glsl")
        },

        fs = {
            checker = getAssetPath("shaders/gl/Checker.frag.glsl"),
            textureWithLighting = getAssetPath("shaders/gl/TextureWithLighting.frag.glsl"),
            grayscale = getAssetPath("shaders/gl/Grayscale.frag.glsl"),
            saturate = getAssetPath("shaders/gl/Saturate.frag.glsl"),
            verticalBlur = getAssetPath("shaders/gl/VerticalBlur.frag.glsl"),
            horizontalBlur = getAssetPath("shaders/gl/HorizontalBlur.frag.glsl"),
            texture = getAssetPath("shaders/gl/Texture.frag.glsl"),
            color = getAssetPath("shaders/gl/Color.frag.glsl"),
            stitches = getAssetPath("shaders/gl/Stitches.frag.glsl")
        }
    }

    return {
        simpleTexture = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.basic, shaderFiles.fs.texture),
        color = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.basic, shaderFiles.fs.color),
        checker = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.basic, shaderFiles.fs.checker),
        basicLighting = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.basicLighting, shaderFiles.fs.textureWithLighting),
        grayscale = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.passThrough, shaderFiles.fs.grayscale),
        saturate = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.passThrough, shaderFiles.fs.saturate),
        verticalBlur = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.passThrough, shaderFiles.fs.verticalBlur),
        horizontalBlur = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.passThrough, shaderFiles.fs.horizontalBlur),
        stitches = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.passThrough, shaderFiles.fs.stitches)
    }
end