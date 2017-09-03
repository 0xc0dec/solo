--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)
local r = scene:createNode():addComponent("SkyboxRenderer")
local tex = sl.CubeTexture.create(sl.device, 1, sl.TextureFormat.RGB)

r:setTexture(tex)
assert(r:getTexture())
r:setTexture(nil)