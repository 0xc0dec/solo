runTest(function()
	local scene = device:getScene()
	local node = scene:createNode()

	assert(node)
	assert(node:getScene())
	assert(node:getId())

	assert(node:findComponent("Transform"))
	assert(node:findComponent("werwerwer") == nil)
	assert(node:findScript("werwer") == nil)

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

			terminate = function()
			end
		}
	end

	local cmp = create()
	node:addScript(cmp)
	assert(node:findScript(cmp.typeId) == cmp)

	cmp = create()
	cmp.typeId = "AnotherComponent"
	node:addScript(cmp)
	assert(node:findScript("AnotherComponent") == cmp)

	local cmp2 = create()
	cmp2.typeId = "YetAnotherComponent"
	node:addScript(cmp2)

	node:removeScript("TestComponent")
	node:removeScript("AnotherComponent")
	node:clearComponents()
end, "Node")
