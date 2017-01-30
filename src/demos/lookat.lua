return function(targetPos)
    return {
        typeId = solo.getCmpId("LookAt"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
        end,

        update = function(self)
            self.transform:lookAt(targetPos, solo.Vector3(0, 1, 0))
        end
    }
end