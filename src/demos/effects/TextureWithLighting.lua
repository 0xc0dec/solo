{
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4",
                invTranspWorld = "mat4"
            }
        },

        inputs = {
            position = "vec3",
            normal = "vec3",
            texCoord = "vec2"
        },

        outputs = {
            uv = "vec2",
            n = "vec3"
        },

        entry = [[
            uv = texCoord;
            n = normalize((#matrices:invTranspWorld# * vec4(normal, 1)).xyz);
            gl_Position = #matrices:wvp# * vec4(position, 1);
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
            vec4 color = texture(mainTex, uv);
            fragColor = color * dot(vec3(1, 1, 1), n) / (length(vec3(1, 1, 1)) * length(n));
        ]]
    }
}
