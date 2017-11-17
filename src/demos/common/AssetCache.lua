--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(dev)
    local cache = {}

    return {
        getEffect = function(vsPath, fsPath)
            vsPath = getShaderPath(dev, vsPath)
            fsPath = getShaderPath(dev, fsPath)
            local key = vsPath .. fsPath

            if not cache[key] then
                cache[key] = sl.Effect.loadFromFiles(dev, vsPath, fsPath)
            end

            return cache[key]
        end
    }
end