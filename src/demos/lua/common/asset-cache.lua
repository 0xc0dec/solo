--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function()
    local texture1 = sl.Texture2D.fromFile(sl.device, assetPath("textures/waffle-slab.png"), true)
    texture1:setAnisotropyLevel(16)
    texture1:setWrap(sl.TextureWrap.Repeat)

    local texture1Normal = sl.Texture2D.fromFile(sl.device, assetPath("textures/waffle-slab_normal.png"), true)
    texture1Normal:setAnisotropyLevel(16)
    texture1Normal:setWrap(sl.TextureWrap.Repeat)

    local cache = {}

    function getOrAdd(key, factory)
        if not cache[key] then
            cache[key] = factory()
            print('Loaded ' .. key)
        end

        return cache[key]
    end

    return {
        getEffect = function(name)
            local path = assetPath("effects/" .. name .. ".lua")
            return getOrAdd(path, function()
                return sl.Effect.fromDescriptionFile(sl.device, path)
            end)
        end,

        meshes = {
            box = function()
                return getOrAdd("mesh_box", function()
                    local layout = sl.VertexBufferLayout()
                    layout:addAttribute(sl.VertexAttributeUsage.Position)
                    layout:addAttribute(sl.VertexAttributeUsage.Normal)
                    layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
                    return sl.Mesh.fromFile(sl.device, assetPath("meshes/box.dae"), layout)
                end)
            end,

            getQuad = function()
                return getOrAdd("mesh_quad", function()
                    local layout = sl.VertexBufferLayout()
                    layout:addAttribute(sl.VertexAttributeUsage.Position)
                    layout:addAttribute(sl.VertexAttributeUsage.Normal)
                    layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
                    return sl.Mesh.fromFile(sl.device, assetPath("meshes/quad.dae"), layout)
                end)
            end
        },

        textures = {
            texture1 = {
                color = texture1,
                normal = texture1Normal
            }
        }
    }
end