--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(dev)
    local cache = {}

    return function(vertShaderPath, fragShaderPath)
        local key = vertShaderPath .. fragShaderPath

        if not cache[key] then
            local vsPath = getAssetPath(vertShaderPath)
            local fsPath = getAssetPath(fragShaderPath)
            cache[key] = sl.Effect.loadFromFiles(dev, vsPath, fsPath)
        end

        return cache[key]
    end
end