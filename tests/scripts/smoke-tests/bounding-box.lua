runTest(function()
	local v = solo.Vector3(1, 2, 3)
	local bb = solo.BoundingBox(v, v)
	local s = solo.BoundingSphere(v, 1)
	assert(solo.BoundingBox.empty())
	assert(bb:getCenter())
	assert(bb:getCorners())
	assert(bb:intersectsBoundingSphere(s) ~= nil)
	assert(bb:intersectsBoundingBox(bb) ~= nil)
	assert(bb:intersectsFrustum(solo.Frustum()) ~= nil)
	assert(bb:hitByRay(solo.Ray(v, v)) ~= nil)
	assert(bb:intersectPlane(solo.Plane(v, 1)) ~= nil)
	assert(bb:isEmpty() ~= nil)
	bb:mergeBoundingBox(bb)
	bb:mergeBoundingSphere(s)
	bb:transform(solo.Matrix())
	assert(bb.min ~= nil)
	assert(bb.max ~= nil)
end, "BoundingBox")
