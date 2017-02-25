--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(fs, getAssetPath)
    local load = function(fileName) return fs:readBytes(getAssetPath(fileName)) end

    return {
        vs = {
            basic = load("basic.vs.glsl"),
            passThrough = load("pass-through.vs.glsl"),
            basicLighting = load("basic-lighting.vs.glsl")
        },

        fs = {
            checker = load("checker.fs.glsl"),
            textureWithLighting = load("texture-with-lighting.fs.glsl"),
            grayscale = load("grayscale.fs.glsl"),
            saturate = load("saturate.fs.glsl"),
            verticalBlur = load("vertical-blur.fs.glsl"),
            horizontalBlur = load("horizontal-blur.fs.glsl"),
            texture = load("texture.fs.glsl"),
            color = load("color.fs.glsl"),
            stitches = load("stitches.fs.glsl")
        }
    }
end