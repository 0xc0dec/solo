GLOBAL device := engine.getDevice();
GLOBAL scene := engine.getScene();
GLOBAL resManager := engine.getResourceManager();

GLOBAL texMaterial;
GLOBAL checkerMaterial;
GLOBAL texWithLightingMaterial;
GLOBAL texEffect;
GLOBAL renderTargetMaterial;


class RotatorAroundLocalXAxis
{
	var transform;

	def RotatorAroundLocalXAxis(node)
	{
		this.transform := node.findComponent("Transform");
	}

	def update()
	{
		var angle = device.getTimeDelta() * 1.3f;
		this.transform.rotate(unitVector3X(), angle, TransformSpace_Self);
	}
}


class RotatorAroundWorldYAxis
{
	var transform;

	def RotatorAroundLocalXAxis(node)
	{
		this.transform := node.findComponent("Transform");
	}

	def update()
	{
		var angle = device.getTimeDelta();
		this.transform.rotate(unitVector3Y(), angle, TransformSpace_World);
	}
}


def loadTexture(path)
{
	var result := resManager.getOrLoadTexture(path);
	result.generateMipmaps();
	result.setFilterMode(Filter_Linear, Filter_Linear);
	result.setAnisotropyLevel(8);
	return result;
}


def initMaterials()
{
	var tex1 := loadTexture("../data/freeman1.png");
	var tex2 := loadTexture("../data/freeman2.png");

	texEffect := resManager.getOrCreateEffect(vsBasic, fsTexture);
	texMaterial := resManager.createMaterial(texEffect);
	texMaterial.setPolygonFace(PolygonFace_All);
	texMaterial.getParameter("worldViewProjMatrix").bindValue(AutoBinding_WorldViewProjectionMatrix);
	texMaterial.getParameter("mainTex").setValue(tex1);

	var checkerEffect := resManager.getOrCreateEffect(vsBasic, fsChecker);
	checkerMaterial := resManager.createMaterial(checkerEffect);
	checkerMaterial.setPolygonFace(PolygonFace_All);
	checkerMaterial.getParameter("color").setValue(Vector4(1, 1, 0, 1));
	checkerMaterial.getParameter("worldViewProjMatrix").bindValue(AutoBinding_WorldViewProjectionMatrix);

	var textureWithLightingEffect := resManager.getOrCreateEffect(vsBasicLighting, fsTextureWithLighting);
	texWithLightingMaterial := resManager.createMaterial(textureWithLightingEffect);
	texWithLightingMaterial.setPolygonFace(PolygonFace_All);
	texWithLightingMaterial.getParameter("worldViewProjMatrix").bindValue(AutoBinding_WorldViewProjectionMatrix);
	texWithLightingMaterial.getParameter("normalMatrix").bindValue(AutoBinding_InverseTransposedWorldMatrix);
	texWithLightingMaterial.getParameter("mainTex").setValue(tex2);
}


def initRenderTargets()
{
	var renderTarget := resManager.getOrCreateRenderTarget("test");
	var renderTexture := resManager.getOrCreateTexture("RTT");
	renderTexture.setData(ColorFormat_RGB, ByteArray(), 160, 120);
	renderTexture.setFilterMode(Filter_Nearest, Filter_Nearest);
	renderTexture.setWrapMode(WrapMode_Clamp, WrapMode_Clamp);
	var texVector = Texture2DArray();
	texVector.push_back(renderTexture);
	renderTarget.setTextures(texVector);

	renderTargetMaterial := resManager.createMaterial(texEffect);
	renderTargetMaterial.setPolygonFace(PolygonFace_All);
	renderTargetMaterial.getParameter("worldViewProjMatrix").bindValue(AutoBinding_WorldViewProjectionMatrix);
	renderTargetMaterial.getParameter("mainTex").setValue(renderTexture);

	return renderTarget;
}


def initCameras(renderTarget)
{
	var offscreenCameraNode := scene.createNode();
	var offscreenCameraTransform := offscreenCameraNode.findComponent("Transform");
	offscreenCameraTransform.setLocalPosition(Vector3(0, 0, 10));
	var offscreenCamera := offscreenCameraNode.addComponent("Camera");
	offscreenCamera.setClearColor(1, 1, 1, 1);
	offscreenCamera.setNear(0.05f);
	offscreenCamera.setRenderTarget(renderTarget);
	offscreenCamera.setViewport(0, 0, 160, 120);

	var mainCameraNode := scene.createNode();
	var mainCameraTransform := mainCameraNode.findComponent("Transform");
	mainCameraTransform.setLocalPosition(0, 0, 10);
	mainCameraNode.addComponent("Spectator");
	mainCameraNode.addComponent("EscapeWatcher");
	var mainCamera := mainCameraNode.addComponent("Camera");
	mainCamera.setClearColor(0, 0.6f, 0.6f, 1);
	mainCamera.setNear(0.05f);
}


def initModel(url, position, material)
{
	var mesh := resManager.getOrLoadMesh(url);
	var model := resManager.getOrCreateModel();
	model.addMesh(mesh);
	var node := scene.createNode();
	var renderer := node.addComponent("ModelRenderer");
	renderer.setModel(model);
	renderer.setMaterial(0, material);
	node.findComponent("Transform").setLocalPosition(position);
	node.addComponent("RotatorAroundLocalXAxis");
}


def initStaticQuad(position, material)
{
	var quad := createQuad();
	quad.findComponent("ModelRenderer").setMaterial(0, material);
	quad.findComponent("Transform").setLocalPosition(position);

	var canvasSize = device.getCanvasSize();
	quad.findComponent("Transform").setLocalScale(5, 5 * canvasSize.y / canvasSize.x, 1);
}


def createQuad()
{
	var mesh := resManager.getOrCreateMesh();
	var vertices = Vector3Array();
	vertices.push_back(Vector3(-1, -1, 0));
	vertices.push_back(Vector3(-1, 1, 0));
	vertices.push_back(Vector3(1, 1, 0));
	vertices.push_back(Vector3(1, -1, 0));
	var normals = Vector3Array();
	normals.push_back(Vector3(0, 0, -1));
	normals.push_back(Vector3(0, 0, -1));
	normals.push_back(Vector3(0, 0, -1));
	normals.push_back(Vector3(0, 0, -1));
	var uvs = Vector2Array();
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	var indices = U16Array();
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	mesh.setVertices(vertices);
	mesh.setNormals(normals);
	mesh.setUVs(uvs);
	mesh.setIndices(indices);

	var model := resManager.getOrCreateModel();
	model.addMesh(mesh);

	var node := scene.createNode();
	var renderer := node.addComponent("ModelRenderer");
	renderer.setModel(model);

	return node;
}


var materials := initMaterials();
var renderTarget := initRenderTargets();
initCameras(renderTarget);
initModel("../data/monkey_hires.obj", Vector3(0, 0, 0), texWithLightingMaterial);
initStaticQuad(Vector3(0, 7, 0), renderTargetMaterial);