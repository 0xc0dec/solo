--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local fb = sl.FrameBuffer.create(sl.device)
local tex = sl.Texture2d.createEmpty(sl.device, 1, 1, sl.TextureFormat.RGB)

fb:setAttachments({ tex, tex })
assert(fb:getDimensions())
