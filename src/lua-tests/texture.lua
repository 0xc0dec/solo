assert(sl.Texture2D.fromFile(sl.device, assetPath('textures/brickwall.jpg'), true))
local handle = sl.Texture2D.fromFileAsync(sl.device, assetPath('textures/brickwall.jpg'), true)
assert(handle.done)

local tex = sl.Texture2D.empty(sl.device, 100, 100, sl.TextureFormat.Depth24)
assert(tex:dimensions())

handle = sl.CubeTexture.fromFaceFilesAsync(sl.device,
    assetPath('textures/skyboxes/deep-space/+x.png'), assetPath('textures/skyboxes/deep-space/-x.png'),
    assetPath('textures/skyboxes/deep-space/+y.png'), assetPath('textures/skyboxes/deep-space/-y.png'),
    assetPath('textures/skyboxes/deep-space/+z.png'), assetPath('textures/skyboxes/deep-space/-z.png')
)
assert(handle.done)

tex = sl.CubeTexture.fromFaceFiles(sl.device,
    assetPath('textures/skyboxes/deep-space/+x.png'), assetPath('textures/skyboxes/deep-space/-x.png'),
    assetPath('textures/skyboxes/deep-space/+y.png'), assetPath('textures/skyboxes/deep-space/-y.png'),
    assetPath('textures/skyboxes/deep-space/+z.png'), assetPath('textures/skyboxes/deep-space/-z.png')
)

assert(tex:format())
assert(tex:horizontalWrap())
assert(tex:verticalWrap())
assert(tex:minFilter())
assert(tex:magFilter())
assert(tex:mipFilter())
assert(tex:anisotropyLevel() ~= nil)
tex:setWrap(sl.TextureWrap.ClampToEdge)
tex:setHorizontalWrap(sl.TextureWrap.ClampToEdge)
tex:setVerticalWrap(sl.TextureWrap.ClampToEdge)
tex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureFilter.Nearest)
tex:setAnisotropyLevel(12)

assert(tex:depthWrap())
tex:setDepthWrap(sl.TextureWrap.ClampToEdge)