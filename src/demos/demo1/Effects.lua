--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

require "Common"

return function(device)
    local fs = device:getFileSystem()
    local load = function(fileName)
        return fs:readBytes(getAssetPath(fileName))
    end

    local shaders = {
        vs = {
            basic = load("Basic.vs.glsl"),
            passThrough = load("PassThrough.vs.glsl"),
            basicLighting = load("BasicLighting.vs.glsl")
        },

        fs = {
            checker = load("Checker.fs.glsl"),
            textureWithLighting = load("TextureWithLighting.fs.glsl"),
            grayscale = load("Grayscale.fs.glsl"),
            saturate = load("Saturate.fs.glsl"),
            verticalBlur = load("VerticalBlur.fs.glsl"),
            horizontalBlur = load("HorizontalBlur.fs.glsl"),
            texture = load("Texture.fs.glsl"),
            color = load("Color.fs.glsl"),
            stitches = load("Stitches.fs.glsl")
        }
    }

    return {
        simpleTexture = sl.Effect.create(device, shaders.vs.basic, shaders.fs.texture),
        color = sl.Effect.create(device, shaders.vs.basic, shaders.fs.color),
        checker = sl.Effect.create(device, shaders.vs.basic, shaders.fs.checker),
        basicLighting = sl.Effect.create(device, shaders.vs.basicLighting, shaders.fs.textureWithLighting),
        grayscale = sl.Effect.create(device, shaders.vs.passThrough, shaders.fs.grayscale),
        saturate = sl.Effect.create(device, shaders.vs.passThrough, shaders.fs.saturate),
        verticalBlur = sl.Effect.create(device, shaders.vs.passThrough, shaders.fs.verticalBlur),
        horizontalBlur = sl.Effect.create(device, shaders.vs.passThrough, shaders.fs.horizontalBlur),
        stitches = sl.Effect.create(device, shaders.vs.passThrough, shaders.fs.stitches)
    }
end