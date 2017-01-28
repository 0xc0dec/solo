local m = solo.Matrix()

assert(solo.Matrix.getSize() ~= nil)

assert(solo.Matrix.identity())

assert(m:isIdentity() ~= nil)

assert(m:getDeterminant() ~= nil)

assert(m:invert() ~= nil)
m:transpose()

m:resetToIdentity()
m:resetToZero()