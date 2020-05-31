local fs = sl.device:fileSystem()
assert(fs)

assert(fs:readBytes(assetPath('test.file')))
assert(fs:readText(assetPath('test.file')))
assert(fs:readLines(assetPath('test.file')))
fs:iterateLines(assetPath('test.file'), function(line) return true end)

fs:writeBytes('../../../temp/output.file', {string.byte('a'), string.byte('b'), string.byte('c')})
fs:writeLines('../../../temp/output.file', {'abc', 'def'})
