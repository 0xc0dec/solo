{
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            }
        },

        inputs = {
            slPosition = "vec4",
            slTexCoord = "vec2",
            normal = "vec3" -- TODO remove?
        },

        outputs = {
            uv = "vec2"
        },

        entry = [[
            uv = slTexCoord;
            gl_Position = #matrices:wvp# * vec4(slPosition.xyz, 1.0);
            FIX_UV#uv#;
            FIX_Y#gl_Position#;
        ]]
    },

    fragment = {
        samplers = {
            mainTex = "sampler2D"
        },

        outputs = {
            fragColor = "vec4"
        },

        entry = [[
            fragColor = texture(mainTex, uv);
        ]]
    }
}
