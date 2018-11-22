--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function()
    local cobbleStone = sl.Texture2D.fromFile(sl.device, getAssetPath("textures/cobblestone_color.png"), true)
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
            local path = getAssetPath("effects/" .. name .. ".lua")
            return getOrAdd(path, function()
                return sl.Effect.fromDescriptionFile(sl.device, path)
            end)
        end,

        meshes = {
            getBox = function()
                return getOrAdd("mesh_box", function()
                    local layout = sl.VertexBufferLayout()
                    layout:addAttribute(sl.VertexAttributeUsage.Position)
                    layout:addAttribute(sl.VertexAttributeUsage.Normal)
                    layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
                    return sl.Mesh.fromFile(sl.device, getAssetPath("meshes/box.dae"), layout)
                end)
            end,

            getQuad = function()
                return getOrAdd("mesh_quad", function()
                    local layout = sl.VertexBufferLayout()
                    layout:addAttribute(sl.VertexAttributeUsage.Position)
                    layout:addAttribute(sl.VertexAttributeUsage.Normal)
                    layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
                    return sl.Mesh.fromFile(sl.device, getAssetPath("meshes/quad.dae"), layout)
                end)
            end
        },

        textures = {
            cobbleStone = cobbleStone
        }
    }
end