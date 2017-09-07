--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local id11 = sl.getCmpId("Cmp1")
local id21 = sl.getCmpId("Cmp2")

assert(id11 > 0)
assert(id21 > 0)
assert(id11 ~= id21)

local id12 = sl.getCmpId("Cmp1")
local id22 = sl.getCmpId("Cmp2")

assert(id11 == id12)
assert(id21 == id22)