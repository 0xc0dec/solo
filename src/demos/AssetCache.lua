--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local effectTools = require "effect-tools"

return function()
    local cubeMesh = sl.Mesh.createFromPrefab(sl.device, sl.MeshPrefab.Cube)
    local quadMesh = sl.Mesh.createFromPrefab(sl.device, sl.MeshPrefab.Quad)
    
    local cobbleStone = sl.Texture2d.loadFromFile(sl.device, getAssetPath("textures/Cobblestone.png"), true)
    cobbleStone:setAnisotropyLevel(16)

    local effectCache = {}

    return {
        getEffect = function(name)
            local path = "effects/" .. name
            local key = path

            if not effectCache[key] then
                local desc = require(path)
                local src = effectTools.generateEffectSource(desc, sl.device:getMode())
                effectCache[key] = sl.Effect.createFromSource(sl.device, src)
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