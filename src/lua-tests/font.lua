local scene = sl.Scene.empty(sl.device)
local node = scene:createNode()
local cam = node:addComponent('Camera')

local font = sl.Font.loadFromFile(
    sl.device,
    assetPath('fonts/aller.ttf'),
    60, 1024, 1024, string.byte(' '), string.byte('~') - string.byte(' '), 2, 2)

assert(font:atlas())

local glyphInfo = font:glyphInfo(string.byte('a'), 0, 0)
assert(glyphInfo.positions)
assert(glyphInfo.uvs)
assert(glyphInfo.offsetX)
assert(glyphInfo.offsetY)

local mesh = sl.FontMesh.fromFont(sl.device, font)
mesh:setText('Abc')
assert(mesh:mesh())
