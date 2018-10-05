--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function()
    local cobbleStone = sl.Texture2D.loadFromFile(sl.device, getAssetPath("textures/cobblestone_color.png"), true)
    cobbleStone:setAnisotropyLevel(16)
    cobbleStone:setWrap(sl.TextureWrap.Repeat)

    local cache = {}

    function getOrAdd(key, factory)
        if not cache[key] then
            cache[key] = factory()
        end

        return cache[key]
    end

    return {
        getEffect = function(name)
            local path = "../../src/demos/lua/effects/" .. name .. ".lua"
            return getOrAdd(path, function()
                return sl.Effect.loadFromDescriptionFile(sl.device, path)
            end)
        end,

        meshes = {
            getBox = function()
                return getOrAdd("mesh_box", function()
                    local layout = sl.VertexBufferLayout()
                    layout:addSemanticAttribute(sl.VertexAttributeSemantics.Position)
                    layout:addSemanticAttribute(sl.VertexAttributeSemantics.Normal)
                    layout:addSemanticAttribute(sl.VertexAttributeSemantics.TexCoord)
                    return sl.Mesh.loadFromFile(sl.device, getAssetPath("meshes/box.dae"), layout)
                end)
            end,

            getQuad = function()
                return getOrAdd("mesh_quad", function()
                    local layout = sl.VertexBufferLayout()
                    layout:addSemanticAttribute(sl.VertexAttributeSemantics.Position)
                    layout:addSemanticAttribute(sl.VertexAttributeSemantics.Normal)
                    layout:addSemanticAttribute(sl.VertexAttributeSemantics.TexCoord)
                    return sl.Mesh.loadFromFile(sl.device, getAssetPath("meshes/quad.dae"), layout)
                end)
            end
        },

        textures = {
            cobbleStone = cobbleStone
        }
    }
end