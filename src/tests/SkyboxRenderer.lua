--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)
local r = scene:createNode():addComponent("SkyboxRenderer")
local tex = sl.CubeTexture.create(sl.device)

r:setTexture(tex)
assert(r:getTexture())
r:setTexture(nil)