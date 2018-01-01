--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

function getShaderPath(name)
    local path = sl.device:getMode() == sl.DeviceMode.OpenGL and ("shaders/gl/" .. name .. ".glsl") or nil
    path = sl.device:getMode() == sl.DeviceMode.Vulkan and ("shaders/vulkan/" .. name .. ".glsl") or path

    if not path then
        error("Could not calculate shader path for " .. name)
    end

    return getAssetPath(path)
end

return function()
    local cache = {}

    return {
        getEffect = function(name)
            local path = getShaderPath(name)
            local key = path

            if not cache[key] then
                cache[key] = sl.Effect.loadFromFile(sl.device, path)
            end

            return cache[key]
        end
    }
end