local ui = sl.device:debugInterface()

sl.device:update(function()
    local cfg = sl.WindowConfig()
    cfg.title = 'Test window'
    cfg.position = sl.Vector2(1, 2)
    cfg.pivot = sl.Vector2(0, 0)
    cfg.alpha = 0.5
    cfg.autoResize = false
    cfg.movable = false
    cfg.decoration = false

    ui:renderWindow(cfg, function()
        ui:renderText('Some text')
    end)
end)
