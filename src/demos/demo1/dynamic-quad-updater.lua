return function(device, data, mesh)
    local time = 0

    return {
        typeId = solo.getCmpId("DynamicQuadUpdater"),

        update = function()
            time = time + 2 * device:getTimeDelta()
            local offset = 0.3 * math.sin(time)
            data[3] = offset
            data[8] = -offset
            data[13] = offset
            data[18] = -offset
            mesh:updateDynamicVertexBuffer(0, 0, data, 4)
        end
    }
end