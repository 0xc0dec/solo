--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

vec2 = sl.Vector2
vec3 = sl.Vector3
vec4 = sl.Vector4

function getAssetPath(fileName)
    return "../../assets/" .. fileName
end

function getShaderPath(dev, name)
    local path = dev:getMode() == sl.DeviceMode.OpenGL and ("shaders/gl/" .. name .. ".glsl") or nil
    path = dev:getMode() == sl.DeviceMode.Vulkan and ("shaders/vulkan/" .. name .. ".spv") or path

    if not path then
        error("Could not calculate shader path for " .. name)
    end

    return getAssetPath(path)
end    

function callSafe(f)
    local _, err = select(1, pcall(f))
    if err then
        logger:logError(err) -- TODO Avoid global logger dependency
    end
end