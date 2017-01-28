logger = solo.device:getLogger()

function runTest(name, test)
    local _, err = select(1, pcall(test))
    if err then
        logger:logError(name .. ": failed\n" .. err)
    else
        logger:logInfo(name .. ": success")
    end
end

runTest("Radian", function()
    local rad = solo.Radian(1)
    assert(rad:toRawRadian() ~= nil)
    assert(rad:toRawDegree() ~= nil)
end)

runTest("Degree", function()
    local deg = solo.Degree(2)
    assert(deg:toRawRadian() ~= nil)
    assert(deg:toRawDegree() ~= nil)
end)

runTest("Vector2", function()
    local v1 = solo.Vector2(2, 3)
    local v2 = solo.Vector2(4, 5)
    
    assert(v1.x ~= nil)
    assert(v1.y ~= nil)
    
    assert(v1:isUnit() ~= nil)
    assert(v1:isZero() ~= nil)
    
    assert(solo.Vector2.angle(v1, v2) ~= nil)
    
    v1:clamp(v1, v2)
    
    assert(v1:distance(v2) ~= nil)
    assert(v1:distanceSquared(v2) ~= nil)
    
    assert(v1:dot(v2) ~= nil)
    assert(solo.Vector2.dot(v1, v2) ~= nil)
    
    assert(v1:length() ~= nil)
    assert(v1:lengthSquared() ~= nil)
    
    v1:normalize()
    assert(v1:normalized() ~= nil)
end)

runTest("Vector3", function()
    local v1 = solo.Vector3(1, 2, 3)
    local v2 = solo.Vector3(1, 2, 3)
    
    assert(v1.x ~= nil)
    assert(v1.y ~= nil)
    assert(v1.z ~= nil)
    
    assert(v1:isUnit() ~= nil)
    assert(v1:isZero() ~= nil)
    
    assert(solo.Vector3.angle(v1, v2) ~= nil)
    
    v1:clamp(v1, v2)
    
    assert(solo.Vector3.cross(v1, v2) ~= nil)
    
    assert(v1:distance(v2) ~= nil)
    assert(v1:distanceSquared(v2) ~= nil)
    
    assert(v1:dot(v2) ~= nil)
    assert(solo.Vector3.dot(v1, v2) ~= nil)
    
    assert(v1:length() ~= nil)
    assert(v1:lengthSquared() ~= nil)
    
    v1:normalize()
    assert(v1:normalized() ~= nil)
end)

runTest("Vector4", function()
    local v1 = solo.Vector4(1, 2, 3, 4)
    local v2 = solo.Vector4(1, 2, 3, 4)
    
    assert(v1.x ~= nil)
    assert(v1.y ~= nil)
    assert(v1.z ~= nil)
    assert(v1.w ~= nil)
    
    assert(v1:isUnit() ~= nil)
    assert(v1:isZero() ~= nil)
    
    assert(solo.Vector4.angle(v1, v2) ~= nil)
    
    v1:clamp(v1, v2)
    
    assert(v1:distance(v2) ~= nil)
    assert(v1:distanceSquared(v2) ~= nil)
    
    assert(v1:dot(v2) ~= nil)
    assert(solo.Vector4.dot(v1, v2) ~= nil)
    
    assert(v1:length() ~= nil)
    assert(v1:lengthSquared() ~= nil)
    
    v1:normalize()
    assert(v1:normalized())
end)

runTest("Quaternion", function()
    local q = solo.Quaternion()
    
    assert(q.x ~= nil)
    assert(q.y ~= nil)
    assert(q.z ~= nil)
    assert(q.w ~= nil)
    
    assert(solo.Quaternion.createFromAxisAngle(solo.Vector3(1, 2, 3), solo.Radian(1)))
    
    assert(q:isIdentity() ~= nil)
    assert(q:isZero() ~= nil)
    
    q:conjugate()
    q:inverse()
    
    assert(q:normalized())
    q:normalize()

    local v = solo.Vector3(1, 2, 3)
    assert(q:toAxisAngle(v))
    
    assert(solo.Quaternion.lerp(solo.Quaternion(), solo.Quaternion(), 1))
    assert(solo.Quaternion.slerp(solo.Quaternion(), solo.Quaternion(), 1))
    assert(solo.Quaternion.squad(solo.Quaternion(), solo.Quaternion(), solo.Quaternion(), solo.Quaternion(), 1))
end)

runTest("Matrix", function()
    local m = solo.Matrix()
    
    assert(solo.Matrix.getSize() ~= nil)
    
    assert(solo.Matrix.identity())

    assert(m:isIdentity() ~= nil)
    
    assert(m:getDeterminant() ~= nil)
    
    assert(m:invert() ~= nil)
    m:transpose()
    
    m:resetToIdentity()
    m:resetToZero()
end)

logger:logInfo("Script tests finished")
