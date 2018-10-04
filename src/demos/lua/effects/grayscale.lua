{
    vertex = {
        inputs = {
            sl_Position = "vec4",
            sl_TexCoord = "vec2"
        },

        outputs = {
            uv = "vec2"
        },

        code = [[
            void main()
            {
                gl_Position = sl_Position;
                uv = sl_TexCoord;
                SL_FIX_Y#gl_Position#;
            }
        ]]
    },

    fragment = {
        uniformBuffers = {
            variables = {
                leftSeparator = "float",
                rightSeparator = "float"
            }
        },

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
                if (uv.x >= #variables:leftSeparator# && uv.x <= #variables:rightSeparator#)
                {
                    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
                    color = vec4(gray, gray, gray, 1);
                }
                fragColor = color;
            }
        ]]
    }
}
