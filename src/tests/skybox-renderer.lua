local scene = solo.Scene.create(solo.device)
local r = scene:createNode():addComponent("SkyboxRenderer")
local tex = solo.CubeTexture.create(solo.device)

r:setTexture(tex)
assert(r:getTexture())