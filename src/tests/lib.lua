local id11 = solo.getCmpId("Cmp1")
local id21 = solo.getCmpId("Cmp2")

assert(id11 > 0)
assert(id21 > 0)
assert(id11 ~= id21)

local id12 = solo.getCmpId("Cmp1")
local id22 = solo.getCmpId("Cmp2")

assert(id11 == id12)
assert(id21 == id22)