local rectTex = solo.RectTexture.create(solo.device)

rectTex:generateMipmaps()

assert(rectTex:getHorizontalWrapping())
assert(rectTex:getVerticalWrapping())

rectTex:setWrapping(solo.TextureWrapping.Clamp)
rectTex:setVerticalWrapping(solo.TextureWrapping.Clamp)
rectTex:setHorizontalWrapping(solo.TextureWrapping.Clamp)

assert(rectTex:getMinFiltering())
assert(rectTex:getMagFiltering())

rectTex:setFiltering(solo.TextureFiltering.NearestMipmapNearest)
rectTex:setMinFiltering(solo.TextureFiltering.NearestMipmapNearest)
rectTex:setMagFiltering(solo.TextureFiltering.NearestMipmapNearest)

assert(rectTex:getAnisotropyLevel() ~= nil)
rectTex:setAnisotropyLevel(1)

rectTex:setData(solo.TextureFormat.RGB, { 1, 2, 3, 4 }, 2, 2)
assert(rectTex:getSize())


local cubeTex = solo.CubeTexture.create(solo.device)

assert(cubeTex:getDepthWrapping())
cubeTex:setDepthWrapping(solo.TextureWrapping.Clamp)

cubeTex:setData(solo.CubeTextureFace.Front, solo.TextureFormat.RGB, { 1, 2, 3, 4 }, 2, 2)