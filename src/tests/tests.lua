logger = solo.device:getLogger()

function runTest(name, test)
    local _, err = select(1, pcall(test))
    if err then
        logger:logError(name .. ": failed\n" .. err)
    else
        logger:logInfo(name .. ": success")
    end
end

runTest("Enums", function()
    -- assert(solo.KeyCode.A ~= nil)
    -- assert(solo.KeyCode.B ~= nil)
    -- assert(solo.KeyCode.C ~= nil)
    -- assert(solo.KeyCode.D ~= nil)
    -- assert(solo.KeyCode.E ~= nil)
    -- assert(solo.KeyCode.F ~= nil)
    -- assert(solo.KeyCode.G ~= nil)
    -- assert(solo.KeyCode.H ~= nil)
    -- assert(solo.KeyCode.I ~= nil)
    -- assert(solo.KeyCode.J ~= nil)
    -- assert(solo.KeyCode.K ~= nil)
    -- assert(solo.KeyCode.L ~= nil)
    -- assert(solo.KeyCode.M ~= nil)
    -- assert(solo.KeyCode.N ~= nil)
    -- assert(solo.KeyCode.O ~= nil)
    -- assert(solo.KeyCode.P ~= nil)
    -- assert(solo.KeyCode.Q ~= nil)
    -- assert(solo.KeyCode.R ~= nil)
    -- assert(solo.KeyCode.S ~= nil)
    -- assert(solo.KeyCode.T ~= nil)
    -- assert(solo.KeyCode.U ~= nil)
    -- assert(solo.KeyCode.V ~= nil)
    -- assert(solo.KeyCode.W ~= nil)
    -- assert(solo.KeyCode.X ~= nil)
    -- assert(solo.KeyCode.Y ~= nil)
    -- assert(solo.KeyCode.Z ~= nil)
    -- assert(solo.KeyCode.LeftArrow ~= nil)
    -- assert(solo.KeyCode.RightArrow ~= nil)
    -- assert(solo.KeyCode.UpArrow ~= nil)
    -- assert(solo.KeyCode.DownArrow ~= nil)
    -- assert(solo.KeyCode.Escape ~= nil)
    -- assert(solo.KeyCode.Space ~= nil)
    -- assert(solo.KeyCode.Backspace ~= nil)

    -- assert(solo.MouseButton.Left ~= nil)
    -- assert(solo.MouseButton.Middle ~= nil)
    -- assert(solo.MouseButton.Right ~= nil)

    -- assert(solo.DeviceMode.Stub ~= nil)
    -- assert(solo.DeviceMode.OpenGL ~= nil)

    -- assert(solo.PolygonFace.All ~= nil)
    -- assert(solo.PolygonFace.CCW ~= nil)
    -- assert(solo.PolygonFace.CW ~= nil)

    -- assert(solo.AutoBinding.CameraWorldPosition ~= nil)
    -- assert(solo.AutoBinding.InverseTransposedWorldMatrix ~= nil)
    -- assert(solo.AutoBinding.InverseTransposedWorldViewMatrix ~= nil)
    -- assert(solo.AutoBinding.ProjectionMatrix ~= nil)
    -- assert(solo.AutoBinding.ViewMatrix ~= nil)
    -- assert(solo.AutoBinding.ViewProjectionMatrix ~= nil)
    -- assert(solo.AutoBinding.WorldMatrix ~= nil)
    -- assert(solo.AutoBinding.WorldViewMatrix ~= nil)
    -- assert(solo.AutoBinding.WorldViewProjectionMatrix ~= nil)

    -- assert(solo.TransformSpace.Self ~= nil)
    -- assert(solo.TransformSpace.Parent ~= nil)
    -- assert(solo.TransformSpace.World ~= nil)

    -- assert(solo.TextureFiltering.Linear ~= nil)
    -- assert(solo.TextureFiltering.Nearest ~= nil)
    -- assert(solo.TextureFiltering.LinearMipmapLinear ~= nil)
    -- assert(solo.TextureFiltering.LinearMipmapNearest ~= nil)
    -- assert(solo.TextureFiltering.NearestMipmapLinear ~= nil)
    -- assert(solo.TextureFiltering.NearestMipmapNearest ~= nil)

    -- assert(solo.ColorFormat.RGB ~= nil)
    -- assert(solo.ColorFormat.RGBA ~= nil)

    -- assert(solo.TextureWrapping.Clamp ~= nil)
    -- assert(solo.TextureWrapping.Repeat ~= nil)

    assert(solo.PlaneIntersection.Front ~= nil)
    assert(solo.PlaneIntersection.Back ~= nil)
    assert(solo.PlaneIntersection.Intersecting ~= nil)

    -- assert(solo.CubeTextureFace.Front ~= nil)
    -- assert(solo.CubeTextureFace.Back ~= nil)
    -- assert(solo.CubeTextureFace.Left ~= nil)
    -- assert(solo.CubeTextureFace.Right ~= nil)
    -- assert(solo.CubeTextureFace.Top ~= nil)
    -- assert(solo.CubeTextureFace.Bottom ~= nil)

    -- assert(solo.DepthFunction.Never ~= nil)
    -- assert(solo.DepthFunction.Less ~= nil)
    -- assert(solo.DepthFunction.Equal ~= nil)
    -- assert(solo.DepthFunction.LEqual ~= nil)
    -- assert(solo.DepthFunction.Greater ~= nil)
    -- assert(solo.DepthFunction.NotEqual ~= nil)
    -- assert(solo.DepthFunction.GEqual ~= nil)
    -- assert(solo.DepthFunction.Always ~= nil)

    -- assert(solo.PrimitiveType.Lines ~= nil)
    -- assert(solo.PrimitiveType.LineStrip ~= nil)
    -- assert(solo.PrimitiveType.Points ~= nil)
    -- assert(solo.PrimitiveType.TriangleStrip ~= nil)
    -- assert(solo.PrimitiveType.Triangles ~= nil)

    -- assert(solo.VertexBufferLayoutSemantics.Position ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.Normal ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.Color ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.Tangent ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.Binormal ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.TexCoord0 ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.TexCoord1 ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.TexCoord2 ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.TexCoord3 ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.TexCoord4 ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.TexCoord5 ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.TexCoord6 ~= nil)
    -- assert(solo.VertexBufferLayoutSemantics.TexCoord7 ~= nil)

    -- assert(solo.MeshPrefab.Quad ~= nil)
    -- assert(solo.MeshPrefab.Cube ~= nil)

    -- assert(solo.EffectPrefab.Skybox ~= nil)

    -- assert(solo.BlendFactor.Zero ~= nil);
    -- assert(solo.BlendFactor.One ~= nil);
    -- assert(solo.BlendFactor.SrcColor ~= nil);
    -- assert(solo.BlendFactor.OneMinusSrcColor ~= nil);
    -- assert(solo.BlendFactor.DstColor ~= nil);
    -- assert(solo.BlendFactor.OneMinusDstColor ~= nil);
    -- assert(solo.BlendFactor.SrcAlpha ~= nil);
    -- assert(solo.BlendFactor.OneMinusSrcAlpha ~= nil);
    -- assert(solo.BlendFactor.DstAlpha ~= nil);
    -- assert(solo.BlendFactor.OneMinusDstAlpha ~= nil);
    -- assert(solo.BlendFactor.ConstantAlpha ~= nil);
    -- assert(solo.BlendFactor.OneMinusConstantAlpha ~= nil);
    -- assert(solo.BlendFactor.SrcAlphaSaturate ~= nil);
end, "Enums")


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

runTest("TransformMatrix", function()
    local m = solo.TransformMatrix()
    local v = solo.Vector3(1, 2, 3)
    local q = solo.Quaternion()
    local rad = solo.Radian(1)
    local bb = solo.BoundingBox(v, v)
    local bs = solo.BoundingSphere(v, 1)
    local plane = solo.Plane(v, 1)
    local ray = solo.Ray(v, v)
    
    assert(solo.TransformMatrix.createLookAt(v, v, v))
    assert(solo.TransformMatrix.createPerspective(rad, 2, 3, 4))
    assert(solo.TransformMatrix.createOrthographic(1, 2, 3, 4))
    assert(solo.TransformMatrix.createScale(v))
    assert(solo.TransformMatrix.createRotationFromQuaternion(q))
    assert(solo.TransformMatrix.createRotationFromAxisAngle(v, rad))
    assert(solo.TransformMatrix.createRotationX(rad))
    assert(solo.TransformMatrix.createRotationY(rad))
    assert(solo.TransformMatrix.createRotationZ(rad))
    assert(solo.TransformMatrix.createTranslation(v))

    assert(m:getScale() ~= nil)
    assert(m:getRotation() ~= nil)
    assert(m:getTranslation() ~= nil)
    assert(m:getUpVector() ~= nil)
    assert(m:getDownVector() ~= nil)
    assert(m:getLeftVector() ~= nil)
    assert(m:getRightVector() ~= nil)
    assert(m:getForwardVector() ~= nil)
    assert(m:getBackVector() ~= nil)

    m:rotateByQuaternion(q)
    m:rotateByAxisAngle(v, rad)
    m:rotateX(rad)
    m:rotateY(rad)
    m:rotateZ(rad)
    
    m:scaleByScalar(1)
    m:scaleByVector(v)

    m:translate(v)

    assert(m:transformPoint(v))
    assert(m:transformDirection(v))
    assert(m:transformBoundingBox(bb))
    assert(m:transformBoundingSphere(bs))
    assert(m:transformPlane(plane))
    assert(m:transformRay(ray))

    m:decompose(v, q, v)
end)

runTest("BoundingBox", function()
    local v = solo.Vector3(1, 2, 3)
    local bb = solo.BoundingBox(v, v)
    local bs = solo.BoundingSphere(v, 1)

    assert(bb.min ~= nil)
    assert(bb.max ~= nil)

    assert(bb:getCenter())
    assert(bb:getCorners())

    assert(bb:intersectsBoundingSphere(bs) ~= nil)
    assert(bb:intersectsBoundingBox(bb) ~= nil)
    assert(bb:intersectsFrustum(solo.Frustum()) ~= nil)

    assert(bb:hitByRay(solo.Ray(v, v)) ~= nil)

    assert(bb:intersectPlane(solo.Plane(v, 1)) ~= nil)

    assert(bb:isEmpty() ~= nil)

    bb:mergeBoundingBox(bb)
    bb:mergeBoundingSphere(bs)
end)

runTest("BoundingSphere", function()
    local v = solo.Vector3(1, 2, 3)
    local s = solo.BoundingSphere(v, 1)
    local bb = solo.BoundingBox(v, v)

    assert(s.center)
    assert(s.radius)

    assert(s:intersectsBoundingSphere(s) ~= nil)
    assert(s:intersectsBoundingBox(bb) ~= nil)
    assert(s:intersectsFrustum(solo.Frustum()) ~= nil)

    assert(s:hitByRay(solo.Ray(v, v)) ~= nil)

    assert(s:intersectPlane(solo.Plane(v, 1)) ~= nil)

    assert(s:isEmpty() ~= nil)

    s:mergeBoundingBox(bb)
    s:mergeBoundingSphere(s)
end)

runTest("Plane", function()
    local p = solo.Plane(solo.Vector3(1, 2, 3), 1)
    local v = solo.Vector3(1, 2, 3)
    local bs = solo.BoundingSphere(v, 1)
    local bb = solo.BoundingBox(v, v)
    local fr = solo.Frustum()
    local ray = solo.Ray(v, v)

    assert(p:getNormal())
    p:setNormal(v)

    assert(p:getDistance() ~= nil)
    p:setDistance(1)

    assert(p:getDistanceToPoint(v) ~= nil)

    assert(solo.Plane.getCommonPoint(p, p, p))

    assert(p:intersectBoundingBox(bb))
    assert(p:intersectBoundingSphere(bs))
    assert(p:intersectFrustum(fr))
    assert(p:intersectPlane(p))
    assert(p:intersectRay(ray))

    assert(p:isParallel(p) ~= nil)
end)

runTest("Ray", function()
    local v = solo.Vector3(1, 0, 0)
    local ray = solo.Ray(v, v)
    local plane = solo.Plane(v, 1)
    local bb = solo.BoundingBox(v, v)
    local bs = solo.BoundingSphere(v, 1)
    local fr = solo.Frustum()

    assert(ray:getOrigin() ~= nil)
    ray:setOrigin(v)

    assert(ray:getDirection() ~= nil)
    ray:setDirection(v)

    assert(ray:hitBoundingBox(bb) ~= nil)
    assert(ray:hitBoundingSphere(bs) ~= nil)
    assert(ray:hitFrustum(fr) ~= nil)
    assert(ray:hitPlane(plane) ~= nil)
end)

runTest("Frustum", function()
    local f = solo.Frustum()
    local m = solo.Matrix()
    local v = solo.Vector3(1, 1, 1)
    local bb = solo.BoundingBox(v, v)
    local bs = solo.BoundingSphere(v, 1)
    local ray = solo.Ray(v, v)
    local plane = solo.Plane(v, 1)

    assert(f:getNearPlane())
    assert(f:getFarPlane())
    assert(f:getLeftPlane())
    assert(f:getRightPlane())
    assert(f:getTopPlane())
    assert(f:getBottomPlane())

    assert(f:getMatrix())
    f:setMatrix(m)

    assert(f:getCorners())
    assert(f:getNearCorners())
    assert(f:getFarCorners())

    assert(f:intersectsPoint(v) ~= nil)
    assert(f:intersectsBoundingBox(bb) ~= nil)
    assert(f:intersectsBoundingSphere(bs) ~= nil)

    assert(f:hitByRay(ray) ~= nil)
    assert(f:intersectPlane(plane) ~= nil)
end)

logger:logInfo("Script tests finished")
