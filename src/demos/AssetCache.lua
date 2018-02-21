--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function()
    local cubeMesh = sl.Mesh.createFromPrefab(sl.device, sl.MeshPrefab.Cube)
    local quadMesh = sl.Mesh.createFromPrefab(sl.device, sl.MeshPrefab.Quad)
    
    local cobbleStone = sl.Texture2D.loadFromFile(sl.device, getAssetPath("textures/Cobblestone.png"), true)
    cobbleStone:setAnisotropyLevel(16)

    local effectCache = {}

    return {
        getEffect = function(name)
            local path = "../../src/demos/effects/" .. name .. ".lua"
            local key = path

            if not effectCache[key] then
                effectCache[key] = sl.Effect.loadFromDescriptionFile(sl.device, path)
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