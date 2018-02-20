{
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4",
                invTranspWorld = "mat4"
            }
        },

        inputs = {
            sl_Position = "vec3",
            slNormal = "vec3",
            slTexCoord = "vec2"
        },

        outputs = {
            uv = "vec2",
            n = "vec3"
        },

        code = [[
            void main()
            {
                uv = slTexCoord;
                n = normalize((#matrices:invTranspWorld# * vec4(slNormal, 1)).xyz);
                gl_Position = #matrices:wvp# * vec4(sl_Position, 1);
                SL_FIX_UV#uv#;
                SL_FIX_Y#gl_Position#;
            }
        ]]
    },

    fragment = {
        samplers = {
            mainTex = "sampler2D"
        },

        outputs = {
            fragColor = "vec4"
        },

        code = [[
            void main()
            {
                vec4 color = texture(mainTex, uv);
                fragColor = color * dot(vec3(1, 1, 1), n) / (length(vec3(1, 1, 1)) * length(n));
            }
        ]]
    }
}
