local mat = solo.Material.create(solo.device, solo.Effect.create(solo.device, "", ""))
local tex = solo.RectTexture.create(solo.device)

mat:setFloatParameter("p1", 1)
mat:setFloatArrayParameter("p2", { 1, 2 })

mat:setVector2Parameter("p3", solo.Vector2(1, 2))
mat:setVector2ArrayParameter("p4", { solo.Vector2(1, 2), solo.Vector2(1, 2) })

mat:setVector3Parameter("p5", solo.Vector3(1, 2, 3))
mat:setVector3ArrayParameter("p6", { solo.Vector3(1, 2, 3), solo.Vector3(1, 2, 3) })

mat:setVector4Parameter("p7", solo.Vector4(1, 2, 3, 4))
mat:setVector4ArrayParameter("p8", { solo.Vector4(1, 2, 3, 4), solo.Vector4(1, 2, 3, 4) })

mat:setMatrixParameter("p9", solo.Matrix())
mat:setMatrixArrayParameter("p10", { solo.Matrix(), solo.Matrix() })

mat:setTextureParameter("p11", tex)

mat:bindWorldMatrixParameter("p12")
mat:bindViewMatrixParameter("p13")
mat:bindProjectionMatrixParameter("p14")
mat:bindWorldViewMatrixParameter("p15")
mat:bindViewProjectionMatrixParameter("p16")
mat:bindInvTransposedWorldMatrixParameter("p17")
mat:bindInvTransposedWorldViewMatrixParameter("p18")
mat:bindCameraWorldPositionParameter("p19")

assert(mat:getEffect())

assert(mat:getPolygonMode())
mat:setPolygonMode(solo.PolygonMode.Points)

assert(mat:getBlend() ~= nil)
mat:setBlend(true)

assert(mat:getSrcBlendFactor() ~= nil)
assert(mat:getDstBlendFactor() ~= nil)
mat:setBlendFactors(solo.BlendFactor.SrcAlphaSaturate, solo.BlendFactor.SrcAlphaSaturate)

assert(mat:getFaceCull() ~= nil)
mat:setFaceCull(solo.FaceCull.All)

assert(mat:getDepthWrite() ~= nil)
mat:setDepthWrite(true)

assert(mat:getDepthTest() ~= nil)
mat:setDepthTest(true)

assert(mat:getDepthFunction() ~= nil)
mat:setDepthFunction(solo.DepthFunction.Never)