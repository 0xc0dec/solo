--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local rectTex = sl.RectTexture.create(sl.device)

rectTex:generateMipmaps()

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

rectTex:setData(sl.ImageFormat.RGB, { 1, 2, 3, 4 }, 2, 2)
assert(rectTex:getSize())


local cubeTex = sl.CubeTexture.create(sl.device)

assert(cubeTex:getDepthWrapping())
cubeTex:setDepthWrapping(sl.TextureWrapping.Clamp)

cubeTex:setData(sl.CubeTextureFace.Front, sl.ImageFormat.RGB, { 1, 2, 3, 4 }, 2, 2)

assert(sl.Texture.loadRectFromFile)
assert(sl.Texture.loadCubeFromFiles)