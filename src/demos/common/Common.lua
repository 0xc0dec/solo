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

function callSafe(f)
    local _, err = select(1, pcall(f))
    if err then
        logger:logError(err) -- TODO Avoid global logger dependency
    end
end