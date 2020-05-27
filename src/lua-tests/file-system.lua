local fs = sl.device:fileSystem()
assert(fs)

assert(fs:readBytes('../../assets/test.file'))
assert(fs:readText('../../assets/test.file'))
assert(fs:readLines('../../assets/test.file'))
fs:iterateLines('../../assets/test.file', function(line) return true end)

fs:writeBytes('../../temp/output.file', {string.byte('a'), string.byte('b'), string.byte('c')})
fs:writeLines('../../temp/output.file', {'abc', 'def'})
