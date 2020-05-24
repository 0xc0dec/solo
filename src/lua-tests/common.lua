--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

vec2 = sl.Vector2
vec3 = sl.Vector3
vec4 = sl.Vector4

function assetPath(fileName)
    return "../../assets/" .. fileName
end

function callSafe(f)
    local success, err = select(1, pcall(f))
    if err then
        print(err)
    end
end

function setWindowTitle()
    local title = 'Demo [' .. sl.device:renderer():name() .. '] [' .. sl.device:renderer():gpuName() .. ']'
    sl.device:setWindowTitle(title)
end

setWindowTitle()