local loader = solo.device:getAssetLoader()

loader:update()

-- No tests here because all functionality goes through real loaders, which expect valid bytes.
-- Too lazy to do that now.
