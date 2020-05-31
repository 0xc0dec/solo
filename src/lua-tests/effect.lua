assert(sl.Effect.fromDescriptionFile(sl.device, assetPath('effects/test.lua')))
assert(sl.Effect.fromSourceFile(sl.device, assetPath('effects/test.effect')))
assert(sl.Effect.fromSource(sl.device, [[
    // VERTEX

    void main()
    {
        gl_Position = vec4(0, 0, 0, 1);
    }

    // FRAGMENT

    void main()
    {
    }
]]))