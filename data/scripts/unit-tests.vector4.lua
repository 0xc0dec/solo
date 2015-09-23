runTest(function()
	local v1 = solo.Vector4(1, 2, 3, 4)
	local v2 = solo.Vector4(2, 3, 4, 5)
	solo.Vector4.zero()
	solo.Vector4.unit()
	solo.Vector4.unitX()
	solo.Vector4.unitY()
	solo.Vector4.unitZ()
	solo.Vector4.unitW()
	solo.Vector4.angle(v1, v2)
	solo.Vector4.dot(v1, v2)
	v1:dot(v2)
	v1:distance(v2)
	v1:distanceSquared(v2)
	v1:clamp(v1, v2)
	v1:length()
	v1:lengthSquared()
	v1:isUnit()
	v1:isZero()
	v1:normalize()
	v1:normalized()
	v1 = v1:plusScalar(1):addScalar(2):plusVector4(solo.Vector4.unit()):addVector4(solo.Vector4.unit())
	print(v1.x .. v1.y .. v1.z .. v1.w)
end, "Vector4")
