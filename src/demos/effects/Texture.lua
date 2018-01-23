return {
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            }
        },

        inputs = {
            position = "vec4",
            texCoord = "vec2"
        },

        outputs = {
            uv = "vec2"
        },

        entry = [[
            uv = texCoord;
            uv.y = 1 - uv.y; // TODO
            gl_Position = #matrices:wvp# * vec4(position.xyz, 1.0);
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
