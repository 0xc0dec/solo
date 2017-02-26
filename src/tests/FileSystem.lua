--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local fs = solo.device:getFileSystem()

assert(fs:readBytes("abc"))
fs:writeBytes("abc", { 1, 2, 3 })

fs:readText("abc")
assert(fs:readLines("abc"))
fs:iterateLines("abc", function(line) return true end)
fs:writeLines("abc", { "abc", "def" })
