--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local fb = sl.FrameBuffer.create(sl.device)
local tex = sl.RectTexture.create(sl.device)

fb:setAttachments({ tex, tex })
assert(fb:getSize())
