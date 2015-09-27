runTest(function()
	local scene = engine:getScene()
	local node = scene:createNode()

	assert(node)
	assert(node:getScene())
	assert(node:getId())

	assert(node:findComponent("Transform"))
	assert(node:findComponent("werwerwer") == nil)
	assert(node:findScriptComponent("werwer") == nil)

	assert(node:findComponent("Camera") == nil)
	assert(node:addComponent("Camera"))
	assert(node:findComponent("Camera"))
	node:removeComponent("Camera")
	assert(node:findComponent("Camera") == nil)

	function create()
		return {
			typeId = "TestComponent",

			init = function()
			end,

			update = function()
			end,

			render = function()
			end,

			postRender = function()
			end,

			terminate = function()
			end
		}
	end

	local cmp = create()
	node:addScriptComponent(cmp)
	assert(node:findScriptComponent(cmp.typeId) == cmp)

	cmp = create()
	cmp.typeId = "AnotherComponent"
	node:addScriptComponent(cmp)
	assert(node:findScriptComponent("AnotherComponent") == cmp)

	local cmp2 = create()
	cmp2.typeId = "YetAnotherComponent"
	node:addScriptComponent(cmp2)

	node:removeScriptComponent("TestComponent")
	node:removeScriptComponent("AnotherComponent")
	node:removeAllComponents()
end, "Node")
