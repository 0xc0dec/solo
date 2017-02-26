--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(fs, getAssetPath)
    local load = function(fileName) return fs:readBytes(getAssetPath(fileName)) end

    return {
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
end