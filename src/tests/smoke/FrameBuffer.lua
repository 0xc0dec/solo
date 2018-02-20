--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local tex = sl.Texture2D.createEmpty(sl.device, 1, 1, sl.TextureFormat.RGB8)
local fb = sl.FrameBuffer.create(sl.device, { tex, tex })

assert(fb:getDimensions())
