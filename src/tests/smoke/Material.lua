--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local mat = sl.Material.create(sl.device, sl.Effect.createFromPrefab(sl.device, sl.EffectPrefab.Skybox))
local tex = sl.Texture2d.createEmpty(sl.device, 1, 1, sl.TextureFormat.RGB)

mat:setFloatParameter("p1", 1)
mat:setVector2Parameter("p3", sl.Vector2(1, 2))
mat:setVector3Parameter("p5", sl.Vector3(1, 2, 3))
mat:setVector4Parameter("p7", sl.Vector4(1, 2, 3, 4))
mat:setMatrixParameter("p9", sl.Matrix())
mat:setTextureParameter("p11", tex)

mat:bindParameter("p20", sl.BindParameterSemantics.WorldMatrix)

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