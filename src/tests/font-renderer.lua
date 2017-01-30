local scene = solo.Scene.create(solo.device)
local r = scene:createNode():addComponent("FontRenderer")

r:setFont(nil) -- make sure nil is allowed