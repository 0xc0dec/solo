--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local logger = solo.device:getLogger()

logger:logDebug("Test debug msg")
logger:logInfo("Test info msg")
logger:logWarning("Test warning msg")
logger:logError("Test error msg")
logger:logCritical("Test critical msg")
-- Do not make further test write into other file
-- logger:setTargetFile("test") 