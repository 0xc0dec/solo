runTest(function()
	local v1 = solo.Vector4(1, 2, 3, 4)
	local v2 = solo.Vector4(2, 3, 4, 5)
	assert(solo.Vector4.zero())
	assert(solo.Vector4.unit())
	assert(solo.Vector4.unitX())
	assert(solo.Vector4.unitY())
	assert(solo.Vector4.unitZ())
	assert(solo.Vector4.unitW())
	assert(solo.Vector4.angle(v1, v2))
	assert(solo.Vector4.dot(v1, v2))
	assert(v1:dot(v2) ~= nil)
	assert(v1:distance(v2) ~= nil)
	assert(v1:distanceSquared(v2) ~= nil)
	v1:clamp(v1, v2)
	assert(v1:length() ~= nil)
	assert(v1:lengthSquared() ~= nil)
	assert(v1:isUnit() ~= nil)
	assert(v1:isZero() ~= nil)
	v1:normalize()
	assert(v1:normalized())
	assert(v1:plusScalar(1):addScalar(2):plusVector4(v2):addVector4(v2))
	assert(v1:productScalar(1):multScalar(1))
	assert(v1.x)
	assert(v1.y)
	assert(v1.z)
end, "Vector4")
