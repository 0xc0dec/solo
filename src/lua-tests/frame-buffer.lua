local tex = sl.Texture2D.empty(sl.device, 100, 100, sl.TextureFormat.Depth24)

local fb = sl.FrameBuffer.fromAttachments(sl.device, { tex })
assert(fb)

assert(fb:dimensions())