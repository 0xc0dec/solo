print("Running Device tests...")

local engine = solo.engine
local device = engine:getDevice()

device:getWindowTitle()
device:setWindowTitle("Solo engine window")
device:getLifetime()

print("Finished Device tests")
