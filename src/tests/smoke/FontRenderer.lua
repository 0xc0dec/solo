--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)
local r = scene:createNode():addFontRenderer()

r:setFont(nil) -- make sure nil is allowed