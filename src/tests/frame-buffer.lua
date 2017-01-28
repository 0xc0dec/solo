local fb = solo.FrameBuffer.create(solo.device)
local tex = solo.RectTexture.create(solo.device)

fb:setAttachments({ tex, tex })
assert(fb:getSize())
