return function(targetPos)
    return {
        typeId = 5,

        init = function(self)
            self.transform = self.node:findComponent("Transform")
        end,

        update = function(self)
            self.transform:lookAt(targetPos, solo.Vector3(0, 1, 0))
        end
    }
end