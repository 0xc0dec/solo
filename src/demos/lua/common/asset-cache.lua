--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function()
    function loadTexture(path)
        local tex = sl.Texture2D.fromFile(sl.device, assetPath(path), true)
        tex:setAnisotropyLevel(16)
        tex:setWrap(sl.TextureWrap.Repeat)
        return tex
    end

    local cache = {}

    function getOrAdd(key, factory)
        if not cache[key] then
            cache[key] = factory()
            print('Loaded ' .. key)
        end

        return cache[key]
    end

    return {
        effect = function(name)
            local path = assetPath('effects/' .. name .. '.lua')
            return getOrAdd(path, function()
                return sl.Effect.fromDescriptionFile(sl.device, path)
            end)
        end,

        meshes = {
            box = function()
                return getOrAdd('mesh_box', function()
                    local layout = sl.VertexBufferLayout()
                    layout:addAttribute(sl.VertexAttributeUsage.Position)
                    layout:addAttribute(sl.VertexAttributeUsage.Normal)
                    layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
                    layout:addAttribute(sl.VertexAttributeUsage.Tangent)
                    return sl.Mesh.fromFile(sl.device, assetPath('meshes/box.dae'), layout)
                end)
            end,

            quad = function()
                return getOrAdd('mesh_quad', function()
                    local layout = sl.VertexBufferLayout()
                    layout:addAttribute(sl.VertexAttributeUsage.Position)
                    layout:addAttribute(sl.VertexAttributeUsage.Normal)
                    layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
                    return sl.Mesh.fromFile(sl.device, assetPath('meshes/quad.dae'), layout)
                end)
            end,

            axes = function()
                return getOrAdd('axes', function()
                    local layout = sl.VertexBufferLayout()
                    layout:addAttribute(sl.VertexAttributeUsage.Position)
                    return sl.Mesh.fromFile(sl.device, assetPath('meshes/axes.obj'), layout)
                end)
            end
        },

        textures = {
            texture1 = {
                color = loadTexture('textures/wood-gate.jpg'),
                normal = loadTexture('textures/wood-gate_normal.jpg')
            },

            texture2 = {
                color = loadTexture('textures/waffle-slab.png'),
                normal = loadTexture('textures/waffle-slab_normal.png')
            },

            house = {
                color = loadTexture('textures/house.png'),
                normal = loadTexture('textures/house_normal.jpg')
            }
        }
    }
end