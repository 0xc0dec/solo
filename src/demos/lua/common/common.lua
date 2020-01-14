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
    local title = 'Demo'

    if sl.device:mode() == sl.DeviceMode.OpenGL then
        title = title .. ' [OpenGL]'
    elseif sl.device:mode() == sl.DeviceMode.Vulkan then
        title = title .. ' [Vulkan]'
    end

    sl.device:setWindowTitle(title .. ' [' .. sl.device:renderer():gpuName() .. ']')
end

setWindowTitle()