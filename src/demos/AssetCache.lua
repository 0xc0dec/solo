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
    local cubeMesh = sl.Mesh.createFromPrefab(sl.device, sl.MeshPrefab.Cube)
    local quadMesh = sl.Mesh.createFromPrefab(sl.device, sl.MeshPrefab.Quad)
    
    local cobbleStone = sl.Texture2d.loadFromFile(sl.device, getAssetPath("textures/Cobblestone.png"), true)
    cobbleStone:setAnisotropyLevel(16)

    local effectCache = {}

    return {
        getEffect = function(name)
            local path = getShaderPath(name)
            local key = path

            if not effectCache[key] then
                effectCache[key] = sl.Effect.loadFromFile(sl.device, path)
            end

            return effectCache[key]
        end,

        meshes = {
            cube = cubeMesh,
            quad = quadMesh
        },

        textures = {
            cobbleStone = cobbleStone
        }
    }
end