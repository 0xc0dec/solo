print("Running BoundingSphere tests...")

runTest(function()
	local v = solo.Vector3(1, 2, 3)
	local s = solo.BoundingSphere(v, 1)
	local bb = solo.BoundingBox(v, v)
	solo.BoundingSphere.empty()
	s:intersectsBoundingSphere(s)
	s:intersectsBoundingBox(bb)
	s:intersectsFrustum(solo.Frustum())
	s:getRayIntersection(solo.Ray(v, v))
	s:getPlaneIntersection(solo.Plane(v, 1))
	s:isEmpty()
	s:mergeBoundingBox(bb)
	s:merge(s)
	s:transform(solo.Matrix())
	local c = s.center
	local r = s.radius
end)

print("Finished BoundingSphere tests")
