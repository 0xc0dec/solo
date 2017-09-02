local gi = sl.GlyphInfo()
assert(gi.positions ~= nil)
assert(gi.uvs ~= nil)
assert(gi.offsetX ~= nil)
assert(gi.offsetY ~= nil)

assert(sl.Font.loadFromFile)
-- TODO test methods