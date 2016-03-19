runTest(function()
    local v = solo.Vector3.unitX()
    local ray = solo.Ray(v, v)
    assert(ray:getOrigin() ~= nil)
    ray:setOrigin(v)
    assert(ray:getDirection() ~= nil)
    ray:setDirection(v)
    assert(ray:hitBoundingBox(solo.BoundingBox.empty()) ~= nil)
    assert(ray:hitBoundingSphere(solo.BoundingSphere.empty()) ~= nil)
    assert(ray:hitPlane(solo.Plane(v, 1)) ~= nil)
    ray:transform(solo.Matrix())
end, "Ray")
