--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local fb = sl.FrameBuffer.create(sl.device)
local tex = sl.RectTexture.create(sl.device, 1, 1, sl.TextureFormat.RGB)

fb:setAttachments({ tex, tex })
assert(fb:getDimensions())
