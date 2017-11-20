--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

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