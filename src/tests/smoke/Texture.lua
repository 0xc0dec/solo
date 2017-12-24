--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

assert(sl.Texture2d.loadFromFile)

local rectTex = sl.Texture2d.createEmpty(sl.device, 1, 1, sl.TextureFormat.RGB)

assert(rectTex:getHorizontalWrapping())
assert(rectTex:getVerticalWrapping())

rectTex:setWrapping(sl.TextureWrapping.Clamp)
rectTex:setVerticalWrapping(sl.TextureWrapping.Clamp)
rectTex:setHorizontalWrapping(sl.TextureWrapping.Clamp)

assert(rectTex:getMinFiltering())
assert(rectTex:getMagFiltering())

rectTex:setFiltering(sl.TextureFiltering.NearestMipmapNearest)
rectTex:setMinFiltering(sl.TextureFiltering.NearestMipmapNearest)
rectTex:setMagFiltering(sl.TextureFiltering.NearestMipmapNearest)

assert(rectTex:getAnisotropyLevel() ~= nil)
rectTex:setAnisotropyLevel(1)

assert(rectTex:getDimensions())

-- Cube textures are currently only loadable from files
-- assert(sl.CubeTexture.loadFromFaceFiles)
-- local cubeTex = sl.CubeTexture.create(sl.device, 1, sl.TextureFormat.RGB)

-- assert(cubeTex:getDepthWrapping())
-- cubeTex:setDepthWrapping(sl.TextureWrapping.Clamp)
