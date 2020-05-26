local effect = sl.Effect.fromDescriptionFile(sl.device, '../../assets/effects/test.lua')
local mat = sl.Material.fromEffect(sl.device, effect)
assert(mat)

mat:setFloatParameter('variables:f', 123)
mat:setVector2Parameter('variables:v2', sl.Vector2(1, 2))
mat:setVector3Parameter('variables:v3', sl.Vector3(1, 2, 3))
mat:setVector4Parameter('variables:v4', sl.Vector4(1, 2, 3, 4))
mat:setMatrixParameter('matrices:wvp', sl.Matrix())

local tex = sl.Texture2D.empty(sl.device, 100, 100, sl.TextureFormat.Depth24)
mat:setTextureParameter('variables:tex', tex)

mat:bindParameter('matrices:wvp', sl.ParameterBinding.WorldViewProjectionMatrix)

assert(mat:effect() == effect)

assert(mat:polygonMode())
mat:setPolygonMode(sl.PolygonMode.Wireframe)

assert(mat:hasBlend() ~= nil)
mat:setBlend(true)

assert(mat:srcBlendFactor())
assert(mat:dstBlendFactor())
mat:setBlendFactors(sl.BlendFactor.Zero, sl.BlendFactor.One)

assert(mat:faceCull() ~= nil)
mat:setFaceCull(sl.FaceCull.Front)

assert(mat:hasDepthWrite() ~= nil)
mat:setDepthWrite(true)

assert(mat:hasDepthTest() ~= nil)
mat:setDepthTest(true)

assert(mat:depthFunction())
mat:setDepthFunction(sl.DepthFunction.Always)