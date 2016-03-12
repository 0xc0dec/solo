runTest(function()
	local v = solo.Vector3(1, 2, 3)
	local v4 = solo.Vector4(1, 2, 3, 4)
	local p = solo.Plane(solo.Vector3(1, 2, 3), 1)
	local q = solo.Quaternion()
	local m = solo.Matrix()
	assert(solo.Matrix.identity())
	assert(solo.Matrix.zero())
	assert(solo.Matrix.createLookAt(v, v, v))
	assert(solo.Matrix.createPerspective(1, 2, 3, 4))
	assert(solo.Matrix.createOrthographic(1, 2, 3, 4))
	assert(solo.Matrix.createOrthographicOffCenter(1, 2, 3, 4, 5, 6))
	assert(solo.Matrix.createBillboard(v, v, v))
	assert(solo.Matrix.createBillboard2(v, v, v, v))
	assert(solo.Matrix.createReflection(p))
	assert(solo.Matrix.createScale(v))
	assert(solo.Matrix.createRotation(q))
	assert(solo.Matrix.createRotationAxisAngle(v, 1))
	assert(solo.Matrix.createRotationX(1))
	assert(solo.Matrix.createRotationY(1))
	assert(solo.Matrix.createRotationZ(1))
	assert(solo.Matrix.createTranslation(v))
	m:decompose(v, q, v)
	assert(m:getDeterminant())
	assert(m:getScale())
	assert(m:getRotation())
	assert(m:getTranslation())
	assert(m:getUpVector())
	assert(m:getDownVector())
	assert(m:getLeftVector())
	assert(m:getRightVector())
	assert(m:getForwardVector())
	assert(m:getBackVector())
	m:invert()
	m:transpose()
	m:rotate(q)
	m:rotateAxisAngle(v, 1)
	m:rotateX(1)
	m:rotateY(2)
	m:rotateZ(3)
	m:scale(1)
	m:scale2(v)
	m:translate(v)
	m:setIdentity()
	m:setZero()
	assert(m:transformPoint(v))
	assert(m:transformVector3Direction(v))
	assert(m:transformVector4Direction(v4))
	assert(m:plusScalar(1):plusMatrix(m):addScalar(2):addMatrix(m))
	assert(m:negative())
	assert(m:minusScalar(1):minusMatrix(m):subtractScalar(2):subtractMatrix(m))
	assert(m:productScalar(1):productMatrix(m):multScalar(2):multMatrix(m))
end, "Matrix")