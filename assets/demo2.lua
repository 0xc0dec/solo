device = solo.device

v1 = solo.Vector2(1, 2)
device:getLogger():logInfo(v1.x .. ", " .. v1.y)