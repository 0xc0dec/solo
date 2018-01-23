{
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            }
        },

        inputs = {
            position = "vec4",
            texCoord = "vec2",
            normal = "vec3"
        },

        outputs = {
            uv = "vec2"
        },

        entry = [[
            uv = texCoord;
            gl_Position = #matrices:wvp# * vec4(position.xyz, 1.0);
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
