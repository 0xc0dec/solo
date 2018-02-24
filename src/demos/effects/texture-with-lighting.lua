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
            sl_Normal = "vec3",
            sl_TexCoord = "vec2"
        },

        outputs = {
            uv = "vec2",
            n = "vec3"
        },

        code = [[
            void main()
            {
                uv = sl_TexCoord;
                n = normalize((#matrices:invTranspWorld# * vec4(sl_Normal, 1)).xyz);
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
            fragColor = { type = "vec4", target = 0 }
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