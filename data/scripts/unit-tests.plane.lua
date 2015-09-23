runTest(function()
	local p = solo.Plane(solo.Vector3(1, 2, 3), 1)
	local v = solo.Vector3(1, 2, 3)
	solo.Plane.intersection(p, p, p)
	local i = solo.Plane.Intersection_Front
	i = solo.Plane.Intersection_Back
	i = solo.Plane.Intersection_Intersecting
	p:getNormal()
	p:setNormal(solo.Vector3(1, 2, 3))
	p:getDistance()
	p:setDistance(1)
	p:getDistanceToPoint(solo.Vector3(1, 2, 3))
	p:isParallel(p)
	p:transform(solo.Matrix())
end, "Plane")
