assert(sl.Texture2D.fromFile(sl.device, '../../assets/textures/cobblestone_color.png', true))
local handle = sl.Texture2D.fromFileAsync(sl.device, '../../assets/textures/cobblestone_color.png', true)
assert(handle.done)

local tex = sl.Texture2D.empty(sl.device, 100, 100, sl.TextureFormat.Depth24)
assert(tex:dimensions())

handle = sl.CubeTexture.fromFaceFilesAsync(sl.device,
    '../../assets/textures/skyboxes/deep-space/+x.png', '../../assets/textures/skyboxes/deep-space/-x.png',
    '../../assets/textures/skyboxes/deep-space/+y.png', '../../assets/textures/skyboxes/deep-space/-y.png',
    '../../assets/textures/skyboxes/deep-space/+z.png', '../../assets/textures/skyboxes/deep-space/-z.png'
)
assert(handle.done)

tex = sl.CubeTexture.fromFaceFiles(sl.device,
    '../../assets/textures/skyboxes/deep-space/+x.png', '../../assets/textures/skyboxes/deep-space/-x.png',
    '../../assets/textures/skyboxes/deep-space/+y.png', '../../assets/textures/skyboxes/deep-space/-y.png',
    '../../assets/textures/skyboxes/deep-space/+z.png', '../../assets/textures/skyboxes/deep-space/-z.png'
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