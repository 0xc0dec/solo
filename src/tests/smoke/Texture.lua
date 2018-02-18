--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

assert(sl.Texture2D.loadFromFile)

local rectTex = sl.Texture2D.createEmpty(sl.device, 1, 1, sl.TextureFormat.RGB)

assert(rectTex:getFormat() ~= nil)

assert(rectTex:getHorizontalWrap())
assert(rectTex:getVerticalWrap())

rectTex:setWrap(sl.TextureWrap.ClampToEdge)
rectTex:setVerticalWrap(sl.TextureWrap.ClampToEdge)
rectTex:setHorizontalWrap(sl.TextureWrap.ClampToEdge)

assert(rectTex:getMinFilter())
assert(rectTex:getMagFilter())
assert(rectTex:getMipFilter())
rectTex:setFilter(sl.TextureFilter.Linear, sl.TextureFilter.Linear, sl.TextureMipFilter.Linear)

assert(rectTex:getAnisotropyLevel() ~= nil)
rectTex:setAnisotropyLevel(1)

assert(rectTex:getDimensions())

-- Cube textures are currently only loadable from files
