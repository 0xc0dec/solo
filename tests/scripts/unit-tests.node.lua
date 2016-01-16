runTest(function()
	local scene = device:getScene()
	local node = scene:createNode()

	assert(node)
	assert(node:getScene())
	assert(node:getId())

	assert(node:findComponent("Transform"))
	assert(node:findComponent("werwerwer") == nil)

	assert(node:findComponent("Camera") == nil)
	assert(node:addComponent("Camera"))
	assert(node:findComponent("Camera"))
	node:removeComponent("Camera")
	assert(node:findComponent("Camera") == nil)

	function create()
		return {
			typeId = 1,

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

	local cmp2 = create()
	cmp2.typeId = 2
	node:addScript(cmp2)

	node:removeScript(cmp)
	node:removeScript(cmp2)
	node:clearComponents()
end, "Node")
