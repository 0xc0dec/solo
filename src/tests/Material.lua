--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local mat = sl.Material.create(sl.device, sl.Effect.create(sl.device, {}, {}))
local tex = sl.RectTexture.create(sl.device, 1, 1, sl.TextureFormat.RGB)

mat:setFloatParameter("p1", 1)
mat:setFloatArrayParameter("p2", { 1, 2 })

mat:setVector2Parameter("p3", sl.Vector2(1, 2))
mat:setVector2ArrayParameter("p4", { sl.Vector2(1, 2), sl.Vector2(1, 2) })

mat:setVector3Parameter("p5", sl.Vector3(1, 2, 3))
mat:setVector3ArrayParameter("p6", { sl.Vector3(1, 2, 3), sl.Vector3(1, 2, 3) })

mat:setVector4Parameter("p7", sl.Vector4(1, 2, 3, 4))
mat:setVector4ArrayParameter("p8", { sl.Vector4(1, 2, 3, 4), sl.Vector4(1, 2, 3, 4) })

mat:setMatrixParameter("p9", sl.Matrix())
mat:setMatrixArrayParameter("p10", { sl.Matrix(), sl.Matrix() })

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
mat:setPolygonMode(sl.PolygonMode.Points)

assert(mat:getBlend() ~= nil)
mat:setBlend(true)

assert(mat:getSrcBlendFactor() ~= nil)
assert(mat:getDstBlendFactor() ~= nil)
mat:setBlendFactors(sl.BlendFactor.SrcAlphaSaturate, sl.BlendFactor.SrcAlphaSaturate)

assert(mat:getFaceCull() ~= nil)
mat:setFaceCull(sl.FaceCull.All)

assert(mat:getDepthWrite() ~= nil)
mat:setDepthWrite(true)

assert(mat:getDepthTest() ~= nil)
mat:setDepthTest(true)

assert(mat:getDepthFunction() ~= nil)
mat:setDepthFunction(sl.DepthFunction.Never)