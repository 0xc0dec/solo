--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

vec2 = sl.Vector2.new
vec3 = sl.Vector3.new
vec4 = sl.Vector4.new

function getAssetPath(fileName)
    return "../../assets/" .. fileName
end

function callSafe(f)
    local success, err = select(1, pcall(f))
    if err then
        print(err)
    end
end