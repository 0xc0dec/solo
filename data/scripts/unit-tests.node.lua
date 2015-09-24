runTest(function()
	local scene = engine:getScene()
	local node = scene:createNode()

	assert(node)
	assert(node:getScene())
	assert(node:getId())

	assert(node:findBuiltInComponent("Transform"))
	assert(node:findBuiltInComponent("werwerwer") == nil)
	assert(node:findComponent("werwer") == nil)

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
	node:addComponent(cmp)
	assert(node:findComponent(cmp.typeId) == cmp)

	cmp = create()
	cmp.typeId = "AnotherComponent"
	node:addComponent(cmp)
	assert(node:findComponent("AnotherComponent") == cmp)

	local cmp2 = create()
	cmp2.typeId = "YetAnotherComponent"
	node:addComponent(cmp2)

	node:removeComponent("TestComponent")
	node:removeComponent("AnotherComponent")
	node:removeAllComponents()
end, "Node")
