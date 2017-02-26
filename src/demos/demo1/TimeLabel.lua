--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createTimeLabelUpdater = dofile("../../src/demos/demo1/TimeLabelUpdater.lua")

return function(dev, fs, scene, tag, getAssetPath)
    local node = scene:createNode()
    node:addScriptComponent(createTimeLabelUpdater(dev, fs, getAssetPath))
    node:findComponent("FontRenderer"):setTag(tag)
    node:findComponent("Transform"):setLocalScale(vec3(0.02, 0.02, 1))
    node:findComponent("Transform"):setLocalPosition(vec3(-3, 0, 4))
end