--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

function getShaderPath(dev, name)
    local path = dev:getMode() == sl.DeviceMode.OpenGL and ("shaders/gl/" .. name .. ".glsl") or nil
    path = dev:getMode() == sl.DeviceMode.Vulkan and ("shaders/vulkan/" .. name .. ".spv") or path

    if not path then
        error("Could not calculate shader path for " .. name)
    end

    return getAssetPath(path)
end

return function(dev)
    local cache = {}

    return {
        getEffect = function(name)
            local vsPath = getShaderPath(dev, name .. ".vert")
            local fsPath = getShaderPath(dev, name .. ".frag")
            local key = vsPath .. fsPath

            if not cache[key] then
                cache[key] = sl.Effect.loadFromFiles(dev, vsPath, fsPath)
            end

            return cache[key]
        end
    }
end