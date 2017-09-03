--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

return function()
    local shaderFiles = {
        vs = {
            basic = getAssetPath("Basic.vs.glsl"),
            passThrough = getAssetPath("PassThrough.vs.glsl"),
            basicLighting = getAssetPath("BasicLighting.vs.glsl")
        },

        fs = {
            checker = getAssetPath("Checker.fs.glsl"),
            textureWithLighting = getAssetPath("TextureWithLighting.fs.glsl"),
            grayscale = getAssetPath("Grayscale.fs.glsl"),
            saturate = getAssetPath("Saturate.fs.glsl"),
            verticalBlur = getAssetPath("VerticalBlur.fs.glsl"),
            horizontalBlur = getAssetPath("HorizontalBlur.fs.glsl"),
            texture = getAssetPath("Texture.fs.glsl"),
            color = getAssetPath("Color.fs.glsl"),
            stitches = getAssetPath("Stitches.fs.glsl")
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