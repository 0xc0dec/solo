device = solo.device
logger = device:getLogger()

v1 = solo.Vector2(1, 2)
logger:logInfo(v1.x .. ", " .. v1.y)

d = solo.Degree(10)
logger:logInfo(d:toRawRadian())